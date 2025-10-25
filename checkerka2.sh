gcc test.c zbior_ary.c -o wzo -std=c17 -pedantic -Wall -Wextra -Wformat-security -Wfloat-equal -Wshadow -Wconversion -Wlogical-not-parentheses -Wnull-dereference -Wvla -Werror -fstack-protector-strong -fsanitize=undefined -fno-sanitize-recover -g -fno-omit-frame-pointer -O1
g++ -std=c++20 brute.cpp -o brute
g++ -std=c++20 gen.cpp -o gen

for((i=12;i<=50000;i++))
do
    echo $i
    ./gen > testy2/in/$i.in
    ./wzo < testy2/in/$i.in > testy2/wzo/$i.wzo
    ./brute < testy2/in/$i.in > testy2/brute/$i.brute
    diff -b testy2/wzo/$i.wzo testy2/brute/$i.brute || break
done