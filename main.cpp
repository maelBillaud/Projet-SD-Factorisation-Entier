#include <iostream>
#include"fint.h"
#include <list>
using namespace std;

int f(int x) {
    return x*x + 1;
}

int pgcd(int a, int b) {
    assert( a > b);
    int c;
    while(b!= 0 ){
        c = a%b;
        a = b;
        b = c;
    }
    return a;
}

int pollard(int n) {
    int_t a = 2, c = 1;
    unsigned long long b = 2; //
    while(c == 1){
        a = f(a) % n;
        cout << "a  = " << a << endl;
        b = f(f(b)) % n;
        cout << "b  = " << b << endl;
        c = pgcd(n, b-a);
        cout << "c  = " << c << endl;
    }
    if (c == -1){return 1;}
    return c;
}


int main() {

    fint test1(40457779);
    cout << "test1 : " << test1 << endl;
    fint test2(40457779, 5);
    cout << "test2 : " << test2 << endl;

    cout << test1.is_prime();
    cout << test2.is_prime();

/*
    int n = 34567568;
    int t;
    for (int i = 0; i < 3; i++) {
        t = pollard(n);
        cout << "facteur : " << t << endl;
        cout << "n : " << n << endl;
        n /= t;
    }
    */
}

