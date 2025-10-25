#include "zbior_ary.h"

#include <stdio.h>
#include <stdlib.h>

zbior_ary tab[1000];


int main(void){

    int n; //liczba operacji do wykonania;
    scanf("%d", &n);

    int Q = -1;

    char type_of_operation;
    char lvalue, rvalue1, rvalue2;
    int a, q, b;

    while(n--){
        scanf(" %c", &type_of_operation); //wazna spacja z przodu!
        switch(type_of_operation){
            case '=': {
                scanf(" %c %d %d %d", &lvalue, &a, &q, &b);
                
                if(Q != -1 && q != Q){
                    printf("Wrong test: Q changed!!!\n");
                    return 0;
                }
                Q = q;

                tab[(int)lvalue] = ciag_arytmetyczny(a, q, b);
                break;
            }
            case '+': {
                scanf(" %c %c %c", &lvalue, &rvalue1, &rvalue2);
                tab[(int)lvalue] = suma(tab[(int)rvalue1], tab[(int)rvalue2]);
                break;
            }
            case '?': {
                scanf(" %c", &lvalue);
                
                if(tab[(int)lvalue].number_of_diff_seq == 0){
                    printf("NULL\n");
                    break;
                }
                for(int seq_index = 0; seq_index < tab[(int)lvalue].number_of_diff_seq; seq_index++){
                    int el = tab[(int)lvalue].min_element[seq_index];
                    while(el <= tab[(int)lvalue].max_element[seq_index]){
                        printf("%d ", el);
                        el += Q;
                    }
                }
                printf("\n");
                break;
            }
            case '-': {
                scanf(" %c %c %c", &lvalue, &rvalue1, &rvalue2);
                tab[(int)lvalue] = roznica(tab[(int)rvalue1], tab[(int)rvalue2]);

                break;
            }
            case '*': {
                scanf(" %c %c %c", &lvalue, &rvalue1, &rvalue2);
                tab[(int)lvalue] = iloczyn(tab[(int)rvalue1], tab[(int)rvalue2]);
                break;
            }
            case '|': {
                scanf(" %c", &lvalue);
                printf("%u\n", moc(tab[(int)lvalue]));
                break;
            }
            case ':': {
                scanf(" %c %d", &lvalue, &b);
                printf("%d\n", nalezy(tab[(int)lvalue], b));
                break;
            }
            case '@': {
                scanf(" %c", &lvalue);
                printf("%d\n", ary(tab[(int)lvalue]));
                break;
            }
            default: {
                printf("Problem! Operation without defined type!\n");
            }
        }
    }

    return 0;
}

