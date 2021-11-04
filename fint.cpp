#include <iostream>
#include <limits>
#include"fint.h"
#include <cassert>
#include <list>
#include <math.h>

using namespace std;

fint::fint(int_t n){
    if (n == 1){
        couple c;
        c.multiplicite = 1;
        c.facteur = 1;
        listFint.push_back(c);
        return;
    }
    list<couple>::iterator it_recup;
    int facteur;
    while(n != 1){
        if (!is_prime(n, facteur)){
            if(estPresent(facteur, it_recup)){
                it_recup->multiplicite++;
            } else {
                couple c;
                c.multiplicite = 1;
                c.facteur = facteur;
                listFint.push_back(c);
            }
            n/= facteur;
        } else {
            if(estPresent(n, it_recup)){
                it_recup->multiplicite++;
            } else {
                couple c;
                c.multiplicite = 1;
                c.facteur = n;
                listFint.push_back(c);
            }
            n = 1;
        }
    }
}


fint::fint(const std::initializer_list<int_t> &lf, const std::initializer_list<int_t> &lm) {
    assert(lf.size() == lm.size());
    list<couple>::iterator it;
    couple c;
    for (auto element : lf){
        c.facteur = element;
        listFint.push_back(c);
    }
    it = listFint.begin();
    for (auto element : lm){
        it->multiplicite = element;
        it++;
    }
}


fint::fint(const fint &copyFint){
    listFint = copyFint.listFint;

}


fint::fint(int_t n, int test) {
    while(n != 1){
        int facteur; // variable qui va recupérer les différentes valeurs des facteurs
        list<couple>::iterator it_recup;
        facteur = pollard(n);
        if (facteur == 1|| n/facteur == 1){ // il se peut que la décomposition soit mal fait et que n ne soit pas vraiment premier
            facteur = decompo_division(n);
        }
        if(estPresent(facteur, it_recup)){
            it_recup->multiplicite++;
        } else {
            couple c;
            c.multiplicite = 1;
            c.facteur = facteur;
            listFint.push_back(c);
        }
        n/= facteur;
    }
}


int_t fint::to_int() const{
    try{
        int_t toInt = 1;
        for(list<couple>::const_iterator it = this->listFint.begin(); it != this->listFint.end(); it++){
            if(pow(it->facteur, it->multiplicite) > MAX_INT_T){
                throw std::underflow_error("Entier trop grand");
            }
            toInt *= pow(it->facteur, it->multiplicite);
        }
        return toInt;
    }

    catch(exception &e)
    {
        cerr << "Caught: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
    };
}


bool fint::divides(const fint& a) const{
    int_t  b = this->to_int();
    int_t  c = a.to_int();
    return((b % c) == 0);
}


bool fint::is_prime() const{
    int n = this->to_int();
    return is_prime(n);
}



bool operator==(const fint& a, const fint& b){
    return (a.to_int() == b.to_int());
}

bool operator!=(const fint& a, const fint& b){
    return (a.to_int() != b.to_int());
}


/*
itération du  premier element au dernier element du fint a comparant chaque facteur
du fint b à partir de son itérateur qui est incrémenté afin d'éviter les comparaisons inutilent
et récupere la premiere valeur qui vérifie que 2 facteur comparés soit égaux
*/
fint lcm(const fint& a, const fint& b){
    std::list<couple>::const_iterator itB;
    itB = b.listFint.begin();
    for(list<couple>::const_iterator itA = a.listFint.begin(); itA != a.listFint.end(); itA++){
        while((itB->facteur<itA->facteur) && (itB != b.listFint.end())){
            itB++;
        }
        if((itB == b.listFint.end()) && (itB->facteur<itA->facteur)){
            fint c(1);
            return c;
        }
        if (itA->facteur == itB->facteur){
            fint c(itA->facteur);

            return c;
        }
    }

    fint c(1);
    return c;
}


/*
itération du  dernière element au premier element du fint a comparant chaque facteur
du fint b à partir de son itérateur qui est incrémenter afin d'éviter les comparaison inutilent
et récupere la premiere valeur qui vérifie que 2 facteurs comparés soit égaux
*/
fint gcd(const fint& a, const fint& b){
    std::list<couple>::const_reverse_iterator itB;
    itB = b.listFint.rbegin();
    for(list<couple>::const_reverse_iterator itA = a.listFint.rbegin(); itA != a.listFint.rend(); itA++){
        while((itB->facteur>itA->facteur) && (itB != b.listFint.rend())){
            itB++;
        }
        if((itB == b.listFint.rbegin()) && (itB->facteur>itA->facteur)){
            fint c(1);
            return c;
        }
        if (itA->facteur == itB->facteur){
            fint c(itA->facteur);

            return c;
        }
    }

    fint c(1);
    return c;
}


/*
itération du  premier element au dernier element du fint b comparant chaque facteur
du fint c qui est une copy de a à partir de son itérateur qui est incrémenté afin d'éviter les comparaisons inutilent.
les facteurs égaut voient leur multipliant additioner dans c
les facteurs inferieurs sont inseré avant ceux dans c
une variable endList permer d'inseré en fin de c si l'iterator de c est en queue et qu'il reste dans b des facteurs superieur à celui dans c
*/
fint operator*(const fint& a, const fint& b){
    fint c(a);
    couple d;
    bool endList = false;
    std::list<couple>::iterator itC;
    itC = c.listFint.begin();
    for(list<couple>::const_iterator itB = b.listFint.begin(); itB != b.listFint.end(); itB++){

        while((itB->facteur > itC->facteur) && (itC != c.listFint.end())){
            itC++;
        }
        if((itC == c.listFint.end()) && (itB->facteur > itC->facteur)){

            d.multiplicite = itB->multiplicite;
            d.facteur = itB->facteur;
            c.listFint.push_back(d);
            endList = true;
        }
        if(!endList){
            if(itB->facteur == itC->facteur){
                itC->multiplicite += itB->multiplicite;
            }else{
                d.multiplicite = itB->multiplicite;
                d.facteur = itB->facteur;
                itC = c.listFint.emplace(itC,d);
            }
        }
    }
    return c;
}


/*
itération du  premier element au dernier element du fint b comparant chaque facteur
du fint c qui est une copy de a à partir de son itérateur qui est incrémenté afin d'éviter les comparaisons inutilent,
la valeur du multipliant du fint c est soustraite par celle de b lorsque 2 facteur égaux sont trouvé, le couple y est suprimer si le multipliant de c atteint 0,
*/
fint operator/(const fint& a, const fint& b){
    try{
        if(a.divides(b)){
            fint c(a);
            std::list<couple>::iterator itC;
            itC = c.listFint.begin();
            for(list<couple>::const_iterator itB = b.listFint.begin(); itB != b.listFint.end(); itB++){
                while(itB->facteur != itC->facteur){
                    itC++;
                }
                if (itB->multiplicite == itC->multiplicite) {
                    itC = c.listFint.erase (itC);
                }else{
                    itC->multiplicite -= itB->multiplicite;
                    itC++;
                }
            }
            if (c.listFint.empty()){
                couple d;
                d.multiplicite = 1;
                d.facteur = 1;
                c.listFint.push_back(d);
            }
            return c;
        }else{
            throw std::domain_error("Division impossible");
        }

    }
    catch(exception &e)
    {
        cerr << "Caught: " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl;
    };

}


//multiplie chaque multiplicité du fint a par n
fint pow(const fint& a, unsigned int n){
    fint b(a);
    for(list<couple>::iterator it = b.listFint.begin(); it != b.listFint.end(); it++){
        it->multiplicite *= n;
    }
    return b;
}


std::ostream& operator<<(std::ostream& os, const fint& a){
    string toString = "";
    for(list<couple>::const_iterator it = a.listFint.begin(); it != a.listFint.end(); it++){
        toString += to_string(it->facteur);
        toString += "^";
        toString += to_string(it->multiplicite);
        if (it != prev(a.listFint.end())){
            toString +=" * ";
        }
    }
    os << toString;
    return os;
}


//----------------------------------Fonctions rajoutées----------------------------------------//


bool fint::estPresent(int f, list<couple>::iterator &it_recup) {
    list<couple>::iterator it;
    for( it = listFint.begin(); it != listFint.end(); it++){
        if (it->facteur == f){
            it_recup = it;
            return true;
        }
    }
    return false;
}


bool fint ::is_prime(int n) const{
    int pas = 4; //pas de la boucle qui va nous permettre de sauter les multiples de 2 et de 3.

    if (n == 2 || n == 3) {
        return true;
    } else {
        if (n % 2 == 0) { // On commence par une conditionnelle pour pouvoir par la suite eliminer tous les multiples de 2.
            return false;
        } else {
            if (n % 3 == 0) { //Même principe mais pour éliminer les multiples de 3.
                return false;
            } else {
                for (int i = 5; i * i < n; i += pas) {
                    if (n % i == 0) {
                        return false;
                    } else {
                        pas = 6 - pas; // permet d'alterner les valeurs de pas entre 2 et 4 pour eliminer tous les multiples de 2 et de 3.
                    }
                }
                return true;
            }
        }
    }
}


bool fint ::is_prime(int n, int &div) const{
    int pas = 4;

    if (n == 2 || n == 3) {
        return true;
    } else {
        if (n % 2 == 0) {
            div = 2;
            return false;
        } else {
            if (n % 3 == 0) {
                div = 3;
                return false;
            } else {
                for (int i = 5; i * i < n; i += pas) {
                    if (n % i == 0) {
                        div = i;
                        return false;
                    } else {
                        pas = 6 - pas;
                    }
                }
                return true;
            }
        }
    }
}


int fint::pgcd(int a, int b) const {
    assert( a > b);
    int c;
    while(b!= 0 ){
        c = a%b;
        a = b;
        b = c;
    }
    return a;
}


int fint::f(int x) const {
    return x*x + 1;
}


int fint::pollard(int n) const {
    int_t a = 2, c = 1;
    int_t b = 2;
    while(c == 1){
        a = f(a) % n;
        b = f(f(b)) % n;
        c = pgcd(n, b-a);
    }
    if (c == -1){return 1;}
    return c;
}


int fint::decompo_division(int n) const {
    int facteur;
    if (is_prime(n), facteur){
        return n;
    } else {
        return n;
    }
}