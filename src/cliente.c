#include <stdio.h>
#include <stdlib.h>
#include "polinomio.h"

int main(){

  Polinomio a = cria_monomio(5, 2);
  Polinomio b = cria_monomio(3, 2);
  Polinomio c = cria_monomio(2.33, 17);
  Polinomio d = cria_monomio(-22873, 17); 
  Polinomio e = soma(a,b);
  Polinomio f = soma(c,d);
  Polinomio g = soma(e,f);



  imprime(a, stdout);
  imprime(b, stdout);
  imprime(c, stdout);
  imprime(d, stdout);
  imprime(e, stdout);
  imprime(f, stdout);
  imprime(g, stdout);


  libera(a);
  libera(b);
  libera(c);
  libera(d);
  libera(e);
  libera(f);
  libera(g);


}
