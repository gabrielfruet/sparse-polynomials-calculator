#include <stdio.h>
#include <stdlib.h>
#include "polinomio.h"

int main(){

  Polinomio a = cria_monomio(2, 3);
  Polinomio b = cria_monomio(3, 4);
  imprime(a, stdout);
  imprime(b, stdout);
  Polinomio c = multiplica(a, b);
  imprime(c, stdout);
  puts("liberando a: ");
  libera(a);
  puts("liberando b: ");
  libera(b);
  puts("liberando c: ");
  libera(c);

  libera_lista();
}
