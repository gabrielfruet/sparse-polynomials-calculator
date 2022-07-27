#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_POL 23
#include "polinomio.h"
#include "calculadora.h"

Polinomio convertePolinomio(char* str){
  Polinomio novo = NULL;
  Polinomio monomio = NULL;
  int tamanho = atoi(strtok(str, " "));
  for(int i = 0; i < tamanho; i++){

    int coef,exp;
    coef = atoi(strtok(NULL, " "));
    exp = atoi(strtok(NULL, " "));

    Polinomio liberar[] = {novo, monomio};

    monomio = cria_monomio(coef, exp);
    novo = soma(novo, monomio);

    for(int i = 0; i < 2; i++){
      libera(liberar[i]);
    }
  }
  return novo;
}

void imprime_baseln(double valor){
  int exp = log10(valor);
  double coef = valor/pow(10, exp);
  printf("%fe+%d\n", coef, exp);
}

int main(){

  Polinomio pols[23];
  FILE* saida = stdout;
  FILE* entrada = fopen("./comandos.txt", "r");

  memset(pols, 0, sizeof(Polinomio)*23);
  char buffer[255];
  while(fgets(buffer,255,entrada) != NULL){
    Polinomio* escolhido = &pols[buffer[0] - 'a'];
    switch(buffer[1]){
      case '?':{
        printf("%c(x) = ", buffer[0]);
        imprime(*escolhido, saida);
      }break;
      case ':':{
        libera(*escolhido);
        *escolhido = convertePolinomio(buffer + 2);      
        printf("%c(x) = ", buffer[0]);
        imprime(*escolhido, saida);
      }break;
      case '=':{
        *escolhido = evaluaExpressao(buffer + 2, pols);
        printf("%c(x) = ", buffer[0]);
        imprime(*escolhido, saida);
      }break;
      case '(':{
        double x = atof(buffer + 2);
        double valor = calcula(*escolhido, x); 
        printf("%c(%.2f) = ", buffer[0], x);
        imprime_baseln(valor);
      }break;
    }
  }

  printf("Finalizando.");
  libera_lista();
}
