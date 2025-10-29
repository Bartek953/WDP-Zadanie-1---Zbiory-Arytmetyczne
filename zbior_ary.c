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
    if(size <= 0){ //na wypadek gdyby size == 0
        size = 1;
    }
    int* result = (int*)malloc((size_t)size * sizeof(int));
    if(!result){
        printf("[MALLOC EREOR] couldn't allocate table\n");
        exit(EXIT_FAILURE);
    }
    return result;
}
zbior_ary create_set(int size){
    zbior_ary result;

    result.allocated_space = size;
    result.min_element = create_table(size);
    result.max_element = create_table(size);

    return result;
}

static int Q; //roznica ciągu
static bool is_q_set = 0;

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
bool are_congruent_mod_q(int x, int y){
    return mod(x) == mod(y);
}
//funkcja wprowadza przyjęty porządek
bool less_in_order(int x, int y){ //czy x mniejszy od y według przyjętego porządku
    if(mod(x) == mod(y)){
        return x < y;
    }
    else {
        return mod(x) < mod(y);
    }
}

zbior_ary ciag_arytmetyczny(int a, int q, int b){
    if(is_q_set == false){
        set_new_q(q);
    }

    zbior_ary result = create_set(1);

    result.number_of_diff_seq = 1;
    result.min_element[0] = a;
    result.max_element[0] = b;

    return result;
}

zbior_ary singleton(int a){
    //singleton to ciąg arytmetyczny o jednym elemencie
    return ciag_arytmetyczny(a, -1, a);
}

zbior_ary copy_set(zbior_ary X){
    zbior_ary result = create_set(X.number_of_diff_seq);
    result.number_of_diff_seq = X.number_of_diff_seq;

    for(int i = 0; i < result.number_of_diff_seq; i++){
        result.min_element[i] = X.min_element[i];
        result.max_element[i] = X.max_element[i];
    }
    return result;
}
void delete_set(zbior_ary* X){
    free(X->min_element);
    free(X->max_element);
    X->min_element = X->max_element = NULL;
}


//funckja zmniejsza zuzycie pamieci do ilosci faktycznych elementow
void reduce_memory(zbior_ary* X){
    zbior_ary result = copy_set(*X);
    //zmniejsza uzycie pamieci - alokuje tylko ilosc faktyznych elementow
    delete_set(X);
    (*X) = result;
}

//funkcja dodaje element na koniec tablicy ciągow i łączy z przedostatnim ciągu
//X - zbior, a - pierwszy element dodawanego ciągu, b - ostatni element dodawanego ciągu
//uwaga, X musi mieć odpowiednio duzą tablice
void push_back(zbior_ary* X, int a, int b){
    assert(X->allocated_space > X->number_of_diff_seq);
    //sprawdzam, czy na pewno mogę dodać elementy do X

    if(X->number_of_diff_seq == 0){
        X->min_element[0] = a;
        X->max_element[0] = b;
        X->number_of_diff_seq++;
        return;
    }

    int n = X->number_of_diff_seq;
    int X_last_max = X->max_element[n - 1];
    //wiemy, ze jesli przystaja mod Q, to X.min_element[n - 1] <= a, bo elementy sa posortowane (według porządku)
    if(are_congruent_mod_q(X_last_max, a) && (a - X_last_max) <= Q){
        //łączymy ciągi
        X->max_element[n - 1] = max(X_last_max, b);
    }
    else {
        //ciągi są rozłączne, nie mozna ich połączyć
        X->min_element[n] = a;
        X->max_element[n] = b;
        X->number_of_diff_seq++;
    }
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

    zbior_ary result = create_set(n + m);

    int index_a = 0;
    int index_b = 0;
    result.number_of_diff_seq = 0;

    //iteruje się po dwoch tablicach, i biorę najmniejszy element
    //inaczej: sortuje dwie posortowane tablice w jedną nową, łącząc ciągi jeśli mogę
    //sortowanie tyczy się przyjętego porządku
    while(index_a < n || index_b < m){
        bool take_from_a = false;
        
        if(index_b == m){
            take_from_a = true;
        }
        else if(index_a < n && less_in_order(A.min_element[index_a], B.min_element[index_b])){
            take_from_a = true;
        }

        if(take_from_a){
            push_back(&result, A.min_element[index_a], A.max_element[index_a]);
            index_a++;
        }
        else {
            push_back(&result, B.min_element[index_b], B.max_element[index_b]);
            index_b++;
        }
    }

    reduce_memory(&result);
    return result;
}

//funkcja usuwa ciąg (a, b) z końca zbioru ciągow X
void pop_back(zbior_ary* X, int a, int b){
    assert(X->allocated_space > X->number_of_diff_seq);
    //sprawdzam, czy na pewno mogę dodać elementy do X

    if(X->number_of_diff_seq == 0){
        //X jest puste, nie ma od czego odjąć
        return;
    }
    int n = X->number_of_diff_seq;
    int X_last_min = X->min_element[n - 1];
    int X_last_max = X->max_element[n - 1];

    if(are_congruent_mod_q(X_last_min, a) == false || b < X_last_min || X_last_max < a){
        //ciągi są rozłączne, nie ma co odejmować
        return;
    }

    X->number_of_diff_seq--;
    //usuwam ostatni ciąg, a potem dodaje te jego części, ktore nie zostały usunięte
    if(X_last_min < a){
        push_back(X, X_last_min, a-Q);
        //dodaję przedział [X_last_min, a)
    }
    if(b < X_last_max){
        push_back(X, b+Q, X_last_max);
        //dodaję przedział (b, X_last_max]
    }
    //innymi słowami z przedziału [X_last_min, X_last_max]
    //"wykrawam" przedział [a, b]
}

zbior_ary roznica(zbior_ary A, zbior_ary B){
    int n = A.number_of_diff_seq;
    int m = B.number_of_diff_seq;
    int indexA = 0;
    int indexB = 0;

    zbior_ary result = create_set(n + m);
    //kazdy element zbioru B moze dodać maksymalnie 1 element do zbioru A
    //(poprzez rozbicie elementu z A na dwa)
    //stąd maksymalna pamięć (n + m)
    result.number_of_diff_seq = 0;

    //będę dodawał elementy z A, a potem "wykrawał" z ncih elementy z B
    //korzystam przy tym, ze elementy w A i B są posortowane według przyjętego porządku
    while(indexA < n || indexB < m){
        if(indexA < n){
            //jeśli mogę, dodaje element z A
            push_back(&result, A.min_element[indexA], A.max_element[indexA]);
            indexA++;
        }
        if(indexB < m){
            //odejmuje element z B
            pop_back(&result, B.min_element[indexB], B.max_element[indexB]);
            //nie inkrementuje indexB, bo ten element moze jeszcze coś usunąć
        }
        
        int res_size = result.number_of_diff_seq;
        int B_curr_max = B.max_element[indexB];
        int res_curr_min = result.min_element[result.number_of_diff_seq - 1];

        while(indexB < m && res_size >= 1 && less_in_order(B_curr_max, res_curr_min)){
            //gdy element B jest za mały by coś z result, to biore następny
            indexB++;
            if(indexB < m){
                //odejmuje element z B, bo moze jest juz aktualny
                pop_back(&result, B.min_element[indexB], B.max_element[indexB]);
            }
            res_size = result.number_of_diff_seq;
            B_curr_max = B.max_element[indexB];
            res_curr_min = result.min_element[result.number_of_diff_seq - 1];
        }

        if(indexA == n){
            //jeśli dodałem juz wszystkie elementy z A, to mogę odjąć pozostałe elementy z B
            while(indexB < m){
                pop_back(&result, B.min_element[indexB], B.max_element[indexB]);
                indexB++;
            }
        }
    }
    reduce_memory(&result);
    return result;
}
zbior_ary iloczyn(zbior_ary A, zbior_ary B) {
    //A * B = A - (A - B)
    zbior_ary diffAB = roznica(A, B);
    zbior_ary result = roznica(A, diffAB);
    delete_set(&diffAB);

    return result;
    //nie trzeba reduce memory, bo result jest wynikiem roznica()
}

unsigned moc(zbior_ary A){
    long long result = 0;
    long long maxi, mini, Qll = Q;

    for(int i = 0; i < A.number_of_diff_seq; i++){
        maxi = A.max_element[i];
        mini = A.min_element[i];
        result += (maxi - mini) / Qll + 1ll;
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
