//
// Created by billa on 24/03/2021.
//

#ifndef PROJET_FINT_H
#define PROJET_FINT_H

#include <iostream>
#include <limits>
#include <list>
#include <math.h>
#include <cassert>
#include <string>


// choix d'un type entier non signé
typedef unsigned long int int_t;

// valeur maximum du type entier choisi
static const int_t MAX_INT_T = std::numeric_limits<int_t>::max();

// ------------------------------------------------------------------
// Définition partielle de la classe fint à compléter SANS MODIFIER
// les signatures des opérations fournies ci-après, ce qui permettra
// au professeur de compiler un programme utilisateur de son choix
// qui respecte cette spécification.
// ------------------------------------------------------------------

// représentation des couples facteurs/multiplicité
typedef struct couple {
    int facteur;
    int multiplicite;
} couple;

class fint {
public:
    // constructeur à partir d'un entier n >= 1
    fint(int_t n);

    // constructeur à partir d'un entier n >= 1 qui utilise le rho de pollard
    // ici test ne sert qu'à différencier ce constructeur du premier
    fint(int_t n , int test);

    //constructeur de copie
    fint(const fint &copyFint);

    // constructeur à partir d'une liste de facteurs premiers lf
    // et d'une liste de multiplicité lm telles que lm et lf ont la
    // même taille et lm[i]>0 est la multiplicité de lf[i] pour tout i
    // exemple d'appel de ce constructeur :
    // fint f({2,5,11}, {1,2,2});
    fint(const std::initializer_list<int_t>& lf, const std::initializer_list<int_t>& lm);



    // destructeur
    //~fint();

    // retourne la valeur décimale de this, throws std::overflow_error
    int_t to_int() const;

    // teste si this divise a
    bool divides(const fint& a) const;

    // teste si this est premier
    bool is_prime() const;

    // comparaisons
    friend bool operator==(const fint& a, const fint& b);
    friend bool operator!=(const fint& a, const fint& b);

    // retourne le plus petit multiple commun de a et b
    friend fint lcm(const fint& a, const fint& b);

    // retourne le plus grand diviseur commun de a et b
    friend fint gcd(const fint& a, const fint& b);

    // retourne a * b
    friend fint operator*(const fint& a, const fint& b);

    // retourne a / b si b divise a, throws std::domain_error sinon
    friend fint operator/(const fint& a, const fint& b);

    // retourne a puissance n
    friend fint pow(const fint& a, unsigned int n);

    // écriture de a sur un flot de sortie
    friend std::ostream& operator<<(std::ostream& os, const fint& a);

    //----------------------------------Fonctions rajoutées----------------------------------------//

    // retourne vrai si l'entier n est premier, sinon faux.
    bool is_prime(int n ) const;

    // retourne vrai si l'entier n est premier et div prend la valeur du plus petit diviseur de n, sinon
    // faux.
    bool is_prime(int n, int &div) const;

    // retourne vrai et met la valeur de l'iterateur vers le couple contenant f dans it_recup si f
    // est un des facteurs contenu dans listFint sinon retourne faux.
    // Cette méthode n'est pas "const" car cela nous aurait obligé à utiliser des
    // _List_const_iterator<couple> ce qui ne nous aurait pas permis changer la valeur d'un element de la
    // liste pointé par cet iterateur.
    bool estPresent(int f, std::list<couple>::iterator &it_recup);

    // retourne le pgcd de deux entiers a et b ou a > b
    int pgcd(int a, int b) const;

    // fonction f, rapide à calculer et pseudo aléatoire, utilisée dans le constructeur utilisant le rho de Pollard
    int f(int x) const;

    // rho de pollard qui renvoie un nombre premier, facteur de n
    int pollard(int n) const;

    // decomposition en produit de facteur premier avec la méthode de la division
    int decompo_division(int n) const;

private:

    std::list<couple> listFint;

};

#endif //PROJET_FINT_H