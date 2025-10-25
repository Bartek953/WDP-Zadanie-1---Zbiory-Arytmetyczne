#include <iostream>
#include <random>
#include <unistd.h>
using namespace std;

//liczby ujemne !!!
//(q > 0)


int main(){
    srand(getpid());

    int Q = 1 + (rand() % 1000);

    int n = 2 + (rand() % 22); //liczba zbiorow - litery alfabetu
    int m = 1 + (rand() % 5000); //liczba operacji na zbiorach

    cout << (3*n + 4*m) << '\n';

    for(int i = 0; i < n; i++){
        int first_el = -1000 + rand()%2100;
        int last_el = first_el + (rand()%400)*Q;
        
        cout << "= " << (char)('A' + i) << ' ' << first_el << ' ' << Q << ' ' << last_el << '\n';
        cout << "? " << (char)('A' + i) << '\n';
        cout << "| " << (char)('A' + i) << '\n';
    }
    for(int i = 0; i < m; i++){
        int typ = rand();
        char zbior1 = 'A' + (rand() % n);
        char zbior2 = 'A' + (rand() % n);
        char zbior3 = 'A' + (rand() % n);

        switch((typ % 6)){
            case 0: { //+ suma teoriomnogosciowa
                cout << "+ " << zbior1 << ' ' << zbior2 << ' ' << zbior3 << '\n';
                break;
            }
            case 1: {//- roznica teoriomnogosciowa
                cout << "- " << zbior1 << ' ' << zbior2 << ' ' << zbior3 << '\n';
                break;
            }
            case 2: {//* iloczyn(przeciecie) teoriomnogosciowe
                cout << "* " << zbior1 << ' ' << zbior2 << ' ' << zbior3 << '\n';
                break;
            }
            case 3: {
                int el = -1000 + rand() % 2100;
                cout << ": " << zbior1 << ' ' << el << '\n';
                break;
            }
            case 4: {
                int el = -10000 + rand() % 21000;
                cout << ": " << zbior1 << ' ' << el << '\n';
                break;
            }
            case 5: {
                int el = -100 + rand() % 210;
                cout << ": " << zbior1 << ' ' << el << '\n';
                break;
            }
        }
        cout << "? " << zbior1 << '\n'; //wypisanie zbioru
        cout << "| " << zbior1 << '\n'; //moc zbioru
        cout << "@ " << zbior1 << '\n'; //funkcja ary()
    }

}