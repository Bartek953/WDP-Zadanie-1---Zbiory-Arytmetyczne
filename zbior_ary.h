#ifndef _ZBIOR_ARY_H_
#define _ZBIOR_ARY_H_

#include <stdbool.h>

typedef struct zbior_ary {
  //skoro kazdy ciąg będzie sumą ciagow arytmetycznych o stałej roznicy
  //to wystarczy pamietac najmniejszy i najwiekszy element kazdego z nich
  //dodatkowo mozemy chciec utrzymywac posortowanie tablic wedlug modulo q, a potem po pierwszym elemencie

  long long* min_element; //tablica najmniejszych elementow
  long long* max_element; //tablica najwiekszych elementow
  //i-ty element tablic t ciąg arytmetyczny od min_element[i] do max_element[i], o roznicy Q

  int number_of_diff_seq; //liczba roznych ciagow arytmetycznych w zbiorze
  int allocated_space;
} zbior_ary;

/* Najlepiej nie modyfikowac nic ponizej. */

// Funkcja daje w wyniku zbior reprezentujacy ciag arytmetyczny o elemencie poczatkowym a, końcowym b i roznicy q>0, tj. {a,a+q,...,b}.
// Mozesz zalozyc, ze we wszystkich zapytaniach w danym tescie wartosc q bedzie taka sama.
// Mozesz zalozyc, ze ta funkcja zostanie wywolana jako pierwsza.
zbior_ary ciag_arytmetyczny(int a, int q, int b);

// Daje w wyniku zbior zlozony z samego elementu a, tj. {a}.
zbior_ary singleton(int a);

// Daje w wyniku zbior reprezentujacy teoriomnogosciowa sume zbiorow A i B.
zbior_ary suma(zbior_ary A, zbior_ary B);

// Daje w wyniku zbior reprezentujacy czesc wspolna zbiorow A i B.
zbior_ary iloczyn(zbior_ary A, zbior_ary B);

// Daje w wyniku zbior A \ B.
zbior_ary roznica(zbior_ary A, zbior_ary B);

bool nalezy(zbior_ary A, int b);
// Daje w wyniku true wtw. gdy liczba b nalezy do zbioru A.

// Wynikiem funkcji jest liczba elementow w zbiorze A.
unsigned moc(zbior_ary A);

// Wynikiem funkcji jest Ary_q(A), czyli minimalna liczba parami rozlacznych ciagow arytmetycznych o roznicy q, ktorych suma jest zbior A.
unsigned ary(zbior_ary A);

#endif
