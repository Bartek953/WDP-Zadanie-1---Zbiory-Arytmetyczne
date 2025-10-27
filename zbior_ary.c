//skoro Q jest stałe, to pojedynczy ciąg arytmetyczny będę reprezentował jako najmniejszy i największy element
//zbior to suma ciagow
//bedzie przechowywany jako tablica najmniejszych i najwiekszych elementow ciagu
//ciagi w tablicy w zbiorze są posortowane najpierw po reszcie z dzielenia przez Q,
//a potem po najmniejszym elemencie
//takie sortowanie będę dalej nazywał przyjętym porządkiem

#include "zbior_ary.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int max(int x, int y){
    if(x >= y){
        return x;
    }
    else {
        return y;
    }
}

int* create_table(int size){
    int* result = (int*)malloc((size_t)size * sizeof(int));

    if(!result){
        printf("[MALLOC ERROR] Couldn't create array!");
        exit(EXIT_FAILURE);
    }
    return result;
}

int Q; //roznica ciągu
bool is_q_set = 0;

//funkcja do testow
void set_new_q(int q){
    Q = q;
    is_q_set = 1;
}

//zwraca reszte z dzielenia x % Q in [0, Q - 1];
int mod(int x){
    int r = x % Q;
    if(r < 0){
        return r + Q;
    }
    else {
        return r;
    }
}

zbior_ary ciag_arytmetyczny(int a, int q, int b){
    if(is_q_set == false){
        set_new_q(q);
    }

    zbior_ary result;
    result.number_of_diff_seq = 1;
    result.allocated_space = 1;

    //konstrukcja tablic
    result.min_element = create_table(result.number_of_diff_seq);
    result.max_element = create_table(result.number_of_diff_seq);

    result.min_element[0] = a;
    result.max_element[0] = b;

    return result;
}

zbior_ary singleton(int a){
    //singleton to ciąg arytmetyczny o jednym elemencie
    return ciag_arytmetyczny(a, -1, a);
}


bool are_congruent_mod_q(int x, int y){
    if(mod(x) == mod(y))
        return 1;
    else
        return 0;
}

void delete_set(zbior_ary* X){
    free(X->min_element);
    free(X->max_element);
    X->max_element = X->min_element = NULL;
}

//funkcja zmniejsza zuzycie pamieci do ilosci faktycznych elementow
void reduce_memory(zbior_ary* X){
    zbior_ary result = copy_set(*X);
    //result alokuje mniej pamieci niz X - tylko tyle ile ma elementow
    delete_set(X);
    (*X) = result;
}

//funkcja dodaje element na koniec tablicy ciągow i łączy z przedostatnim ciągiem
//X - zbior, a - pierwszy element dodawanego ciągu, b - ostatni element dodawanego ciągu
//uwaga, X musi mieć odpowiednio duzą tablice
void push_back(zbior_ary* X, int a, int b){
    if(X->number_of_diff_seq == 0){
        assert(X->allocated_space > 0);
        X->min_element[0] = a;
        X->max_element[0] = b;
        X->number_of_diff_seq++;
        return;
    }

    int n = X->number_of_diff_seq;
    int X_last_el = X->max_element[n - 1];
    //wiemy, ze jesli przystaja mod Q, to X.min_element[n - 1] <= a, bo elementy sa posortowane (według porządku)
    if(are_congruent_mod_q(X_last_el, a) && (a - X_last_el) <= Q){
        //łączymy ciągi
        X->max_element[n - 1] = max(X_last_el, b);
    }
    else {
        assert(X->allocated_space > n);
        X->min_element[n] = a;
        X->max_element[n] = b;
        X->number_of_diff_seq++;
    }
}

bool compare(int x, int y){ //czy x mniejszy od y według przyjętego porządku
    if(mod(x) < mod(y))
        return 1;
    if(mod(x) > mod(y))
        return 0;
    //x mod Q == y mod Q
    return (x < y);
}

zbior_ary suma(zbior_ary A, zbior_ary B){
    //dane są pary (a, q, b), (c, q, d)
    //zauwazmy, ze a(mod q) = b(mod q)
    //i c(mod q) = d(mod q)
    //stąd mamy przypadki:

    //1. a(mod q) != c(mod q) => ciagi sa rozne, nie mozemy ich polaczyc
    //2. a(mod q) == c(mod q) i (b < c lub d < a) => ciagi sa rozne, nie mozemy ich polaczyc
    //wpp mozemy polaczyc ciagi

    //bedziemy utrzymywac posortowanie po a(mod q)
    //a w drugiej kolejnosci po pierwszym elemencie

    int n = A.number_of_diff_seq;
    int m = B.number_of_diff_seq;

    zbior_ary result;
    result.min_element = create_table(n + m);
    result.max_element = create_table(n + m);
    result.allocated_space = n + m;

    int index_a = 0;
    int index_b = 0;
    result.number_of_diff_seq = 0;

    while(index_a < n || index_b < m){
        if(index_a == n){
            //bierzemy element z B (jest mniejszy)
            push_back(&result, B.min_element[index_b], B.max_element[index_b]);
            index_b++;
            continue;
        }
        if(index_b == m){
            //bierzemy element z A (jest mniejszy)
            push_back(&result, A.min_element[index_a], A.max_element[index_a]);
            index_a++;
            continue;
        }

        if(less_in_order(B.min_element[index_b], A.min_element[index_a])){
            //bierzemy element z B (jest mniejszy)
            push_back(&result, B.min_element[index_b], B.max_element[index_b]);
            index_b++;
            continue;
        }
        else {
            //bierzemy element z A (jest mniejszy)
            push_back(&result, A.min_element[index_a], A.max_element[index_a]);
            index_a++;
            continue;
        }
    }

    return result;
}

//funkcja usuwa ciąg (a, b) z końca zbioru ciągow X
void pop_back(zbior_ary* X, int a, int b){
    if(X->number_of_diff_seq == 0){
        return;
    }
    int n = X->number_of_diff_seq;
    int X_last_min = X->min_element[n - 1];
    int X_last_max = X->max_element[n - 1];

    if(are_congruent_mod_q(X_last_min, a) == false || b < X_last_min || X_last_max < a){
        //ostani ciąg X i ciąg (a, b) są rozłączne - nie ma co odejmować
        return;
    }

    //odejmuje najpierw cały ostatni ciąg z X
    //a potem dodaje te części, ktore nie zostały usunięte
    X->number_of_diff_seq--;
    if(X_last_min < a){
        push_back(X, X_last_min, a-Q);
    }
    if(b < X_last_max){
        push_back(X, b+Q, X_last_max);
    }
}

zbior_ary roznica(zbior_ary A, zbior_ary B){
    int n = A.number_of_diff_seq;
    int m = B.number_of_diff_seq;
    int indexA = 0;
    int indexB = 0;

    zbior_ary result;
    result.min_element = create_table(n + m);
    result.max_element = create_table(n + m);
    result.number_of_diff_seq = 0;
    result.allocated_space = n + m;

    while(indexA < n || indexB < m){
        if(indexA < n){
            //dodaje element z A
            push_back(&result, A.min_element[indexA], A.max_element[indexA]);
            indexA++;
        }
        if(indexB < m){
            //odejmuje element z B
            pop_back(&result, B.min_element[indexB], B.max_element[indexB]);
            //nie aktualizuje indexB, bo moze jeszcze zahaczyc o przyszłe elementy A
        }

        int res_size = result.number_of_diff_seq;
        while(indexB < m && res_size >= 1 && less_in_order(B.max_element[indexB], result.min_element[res_size - 1])){
            //jeśli ciąg z B jest mniejszy od ciągu result
            //to aktualizuje indexB, az B będzie aktualne
            indexB++;
            if(indexB < m){
                pop_back(&result, B.min_element[indexB], B.max_element[indexB]);
            }
        }
        if(indexA == n){
            //jesli dodałem juz wszystkie elementy z A,
            //to mogę odjąć wszystko co zostało z B
            while(indexB < m){
                pop_back(&result, B.min_element[indexB], B.max_element[indexB]);
                indexB++;
            }
        }
    }

    return result;
}
zbior_ary iloczyn(zbior_ary A, zbior_ary B) {
    //A * B = A - (A - B)
    zbior_ary diffAB = roznica(A, B);
    zbior_ary result = roznica(A, diffAB);
    delete_set(&diffAB);

    //mozna po prostu A - (A - B)

    return result;
}

unsigned moc(zbior_ary A){
    long long result = 0;
    for(int i = 0; i < A.number_of_diff_seq; i++){
        result += (long long)(A.max_element[i] - A.min_element[i]) / Q + 1ll;
    }
    return (unsigned int)result;
}
unsigned ary(zbior_ary A){
    return (unsigned int)A.number_of_diff_seq;
}

bool nalezy(zbior_ary A, int b){
    //jesli b nalezy do A, to b nalezy do ktoregos z ciagow arytmetycznych A
    //bin search po indeksie ciągu do ktorego nalezy b
    //mozemy to zrobić, bo ciągi są posortowane weług porządku

    if(moc(A) == 0){
        return false;
    }
    int l = -1;
    int r = A.number_of_diff_seq;
    int mid;

    while(l + 1 < r){
        mid = (l + r) / 2;
        if(less_in_order(b, A.min_element[mid])){ //czy b jest mniejsze od ciagu mid
            r = mid;
        }
        else {
            l = mid;
        }
    }

    if(l < 0){
        return false;
    }

    int low_el = A.min_element[l];
    int high_el = A.max_element[l];
    if(are_congruent_mod_q(b, low_el) && low_el <= b && b <= high_el){
        return true;
    }
    else {
        return false;
    }
}
