#include <iostream>
#include <set>
using namespace std;

int Q = -1;

int mod(int x){
    int r = x % Q;
    if(r < 0)
        return r + Q;
    return r;
}

set<int> ciag_arytmetyczny(int a, int q, int b){
    if(Q == -1){
        Q=q;
    }

    set<int> result;
    for(int el = a; el <= b; el += q){
        result.insert(el);
    }
    return result;
}
set<int> singleton(int a){
    return ciag_arytmetyczny(a, Q, a);
}
set<int> suma(set<int> A, set<int> B){
    set<int> result;
    for(int el : A){
        result.insert(el);
    }
    for(int el : B){
        result.insert(el);
    }
    return result;
}
set<int> roznica(set<int> A, set<int> B){
    set<int> result = A;
    for(int el : B){
        result.erase(el);
    }
    return result;
}
set<int> iloczyn(set<int> A, set<int> B){
    set<int> result = A;
    for(auto el : A){
        if(B.count(el) == 0){
            result.erase(el);
        }
    }
    return result;
}
void wypisz(set<int> A){
    if(A.size() == 0){
        cout << "NULL\n";
        return;
    }
    for(int reszta = 0; reszta < Q; reszta++){
        for(int el : A){
            if(mod(el) == reszta){
                cout << el << ' ';
            }
        }
    }
    cout << '\n';
}
long long moc(set<int> A){
    return A.size();
}
bool nalezy(set<int> A, int b){
    return A.count(b) > 0;
}
int ary(set<int> A){
    set<int> X = A;
    int result = 0;
    int element;

    while(!X.empty()){
        element = (*X.begin());
        result++;
        while(X.count(element) > 0){
            X.erase(element);
            element += Q;
        }
    }
    return result;
}

set<int> tab[1000];

int main(){
    int n;
    cin >> n;
    char typ_operacji, lvalue, rvalue1, rvalue2;
    int a, q, b;
    while(n--){
        cin >> typ_operacji;
        switch(typ_operacji){
            case '=': {
                cin >> lvalue >> a >> q >> b;
                tab[lvalue] = ciag_arytmetyczny(a, q, b);
                break;
            }
            case '+': {
                cin >> lvalue >> rvalue1 >> rvalue2;
                tab[lvalue] = suma(tab[rvalue1], tab[rvalue2]);
                break;
            }
            case '-': {
                cin >> lvalue >> rvalue1 >> rvalue2;
                tab[lvalue] = roznica(tab[rvalue1], tab[rvalue2]);
                break;
            }
            case '*': {
                cin >> lvalue >> rvalue1 >> rvalue2;
                tab[lvalue] = iloczyn(tab[rvalue1], tab[rvalue2]);
                break;
            }
            case '?': {
                cin >> lvalue;
                wypisz(tab[lvalue]);
                break;
            }
            case '|': {
                cin >> lvalue;
                cout << moc(tab[lvalue]) << '\n';
                break;
            }
            case ':': {
                cin >> lvalue >> b;
                cout << nalezy(tab[lvalue], b) << '\n';
                break;
            }
            case '@': {
                cin >> lvalue;
                cout << ary(tab[lvalue]) << '\n';
                break;
            }
            default: {
                cout << "ERROR: Incorrect type of operation!\n";
            }
        }
    }
}