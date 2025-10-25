//reprezentacja ciagu jako sumy ciagow teoriomnogosciowych (el poczatkowy i q)
//stale q!
//zmienic unsigned na long longa

//!!!!!
//q w danym tescie jest stale na caly test - wszystkie ciag_arytmetyczny() beda uzywac tego samego q!
//!!!!!

//modulo q?

//uwaga na inty, unsigned i long longi

#include "zbior_ary.h"

//#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include <math.h>

int max(int x, int y){
    if(x >= y)
        return x;
    return y;
}

int* create_table(int size){
    return (int*)malloc((size_t)size * sizeof(int));
}

int Q; //roznica ciągu
int is_q_set = 0; //bool

//funkcja do testow
void set_new_q(int q){
    Q = q;
    is_q_set = 1;
}

//zwraca reszte z dzielenia x % Q in [0, Q - 1];
int mod(int x){
    int r = x % Q;
    if(r < 0)
        return r + Q;
    else
        return r;
}

zbior_ary ciag_arytmetyczny(int a, int q, int b){
    if(is_q_set == 0){
        set_new_q(q);
    }

    zbior_ary result;
    result.number_of_diff_seq = 1;
    result.allocated_space = 1;

    //kontrukcja tablic
    result.first_element = create_table(result.number_of_diff_seq);
    result.last_element = create_table(result.number_of_diff_seq);

    result.first_element[0] = a;
    result.last_element[0] = b;

    return result;
}

zbior_ary singleton(int a){
    //singleton to sczczegolny przypadek ciagu arytmetycznego,
    //gdzie pierwszy element jest rowny ostatniemu

    //singleton bedziemy oznaczac jako zbior arytmetyczny z q=-1

    return ciag_arytmetyczny(a, -1, a);
}


//zwraca bool
int are_congruent_mod_q(int x, int y){
    if(mod(x) == mod(y))
        return 1;
    else
        return 0;
}

//funkcja dodaje element na koniec tablicy ciągow i łączy z przedostatnim ciągu
//X - zbior, a - pierwszy element dodawanego ciągu, b - ostatni element dodawanego ciągu
//uwaga, X musi mieć odpowiednio duzą tablice
void push_back(zbior_ary* X, int a, int b){
    if(X->number_of_diff_seq == 0){
        assert(X->allocated_space > 0);
        X->first_element[0] = a;
        X->last_element[0] = b;
        X->number_of_diff_seq++;
        return;
    }

    int n = X->number_of_diff_seq;
    int X_last_el = X->last_element[n - 1];
    //wiemy, ze jesli przystaja mod Q, to X.first_element[n - 1] <= a, bo elementy sa posortowane
    if(are_congruent_mod_q(X_last_el, a) && (a - X_last_el) <= Q){
        X->last_element[n - 1] = max(X_last_el, b);
    }
    else {
        assert(X->allocated_space > n);
        X->first_element[n] = a;
        X->last_element[n] = b;
        X->number_of_diff_seq++;
    }
}

int compare(int x, int y){ //czy x mniejszy od y - zwraca bool
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
    result.first_element = create_table(n + m);
    result.last_element = create_table(n + m);
    result.allocated_space = n + m;

    int index_a = 0;
    int index_b = 0;
    result.number_of_diff_seq = 0;

    while(index_a < n || index_b < m){
        if(index_a == n){
            //bierzemy element z B (jest mniejszy)
            push_back(&result, B.first_element[index_b], B.last_element[index_b]);
            index_b++;
            continue;
        }
        if(index_b == m){
            //bierzemy element z A (jest mniejszy)
            push_back(&result, A.first_element[index_a], A.last_element[index_a]);
            index_a++;
            continue;
        }
        if(compare(B.first_element[index_b], A.first_element[index_a])){
            //bierzemy element z B (jest mniejszy)
            push_back(&result, B.first_element[index_b], B.last_element[index_b]);
            index_b++;
            continue;
        }
        else {
            //bierzemy element z A (jest mniejszy)
            push_back(&result, A.first_element[index_a], A.last_element[index_a]);
            index_a++;
            continue;
        }
        //...
    }

    return result;
}

void pop_back(zbior_ary* X, int a, int b){
    if(X->number_of_diff_seq == 0)
        return;
    int n = X->number_of_diff_seq;
    int X_last_low = X->first_element[n - 1];
    int X_last_high = X->last_element[n - 1];

    if(are_congruent_mod_q(X_last_low, a) == 0 || b < X_last_low || X_last_high < a)
        return;

    X->number_of_diff_seq--;
    if(X_last_low < a){
        push_back(X, X_last_low, a-Q);
    }
    if(b < X_last_high){
        push_back(X, b+Q, X_last_high);
    }
}

void printSet(zbior_ary X){
    for(int i = 0; i < X.number_of_diff_seq; i++){
        int el = X.first_element[i];
        while(el <= X.last_element[i]){
            printf("%d ", el);
            el += Q;
        }
        printf(" : ");
    }
    printf("\nRoznych:%d\n", X.number_of_diff_seq);
}

zbior_ary roznica(zbior_ary A, zbior_ary B){
    int n = A.number_of_diff_seq;
    int m = B.number_of_diff_seq;
    int indexA = 0;
    int indexB = 0;

    zbior_ary result;
    result.first_element = create_table(n + m);
    result.last_element = create_table(n + m);
    result.number_of_diff_seq = 0;
    result.allocated_space = n + m;

    while(indexA < n || indexB < m){
        if(indexA < n){
            push_back(&result, A.first_element[indexA], A.last_element[indexA]);
            indexA++;
        }
        if(indexB < m){
            pop_back(&result, B.first_element[indexB], B.last_element[indexB]);
        }
        while(indexB < m && result.number_of_diff_seq >= 1 && compare(B.last_element[indexB], result.first_element[result.number_of_diff_seq - 1])){
            indexB++;
            if(indexB < m){
                pop_back(&result, B.first_element[indexB], B.last_element[indexB]);
            }
        }
        if(indexA == n){
            while(indexB < m){
                pop_back(&result, B.first_element[indexB], B.last_element[indexB]);
                indexB++;
            }
        }
    }

    return result;
}
zbior_ary iloczyn(zbior_ary A, zbior_ary B) {
    zbior_ary result = suma(A, B);
    zbior_ary diffAB = roznica(A, B);
    zbior_ary diffBA = roznica(B, A);
    
    result = roznica(result, diffAB);
    result = roznica(result, diffBA);

    //mozna po prostu A - (A - B)

    return result;
}

unsigned moc(zbior_ary A){
    long long result = 0;
    for(int i = 0; i < A.number_of_diff_seq; i++){
        result += (long long)(A.last_element[i] - A.first_element[i]) / Q + 1ll;
    }
    return (unsigned int)result;
}
unsigned ary(zbior_ary A){
    return (unsigned int)A.number_of_diff_seq;
}

bool nalezy(zbior_ary A, int b){
    //jesli b nalezy do A, to b nalezy do ktoregos z ciagow arytmetycznych A
    //bin search
    if(moc(A) == 0)
        return false;
    int l = -1;
    int r = A.number_of_diff_seq;
    int mid;

    while(l + 1 < r){
        mid = (l + r) / 2;
        if(compare(b, A.first_element[mid])) //czy b jest mniejsze od ciagu mid
            r = mid;
        else
            l = mid;
    }
    if(l < 0)
        return false;

    int low_el = A.first_element[l];
    int high_el = A.last_element[l];
    if(are_congruent_mod_q(b, low_el) && low_el <= b && b <= high_el)
        return true;
    else
        return false;
}
