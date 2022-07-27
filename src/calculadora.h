#ifndef __CALCULADORA_POLINOMIOS
#define __CALCULADORA_POLINOMIOS
#include "polinomio.h"
char* infixoParaPosfixo(char* expr);
Polinomio evaluaExpressao(char* infix, Polinomio pols[23]);
#endif
