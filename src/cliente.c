#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_POL 23
#include "polinomio.h"
#include "calculadora.h"

size_t totalBytes(Polinomio pols[23]);

Polinomio convertePolinomio(char* str){
  int i, j;
  Polinomio novo = NULL;
  Polinomio monomio = NULL;
  int tamanho = atoi(strtok(str, " "));
  for(i = 0; i < tamanho; i++){

    double coef;
    int exp;

    Polinomio liberar[2];

    coef = atof(strtok(NULL, " "));
    exp = atoi(strtok(NULL, " "));


    monomio = cria_monomio(coef, exp);

    liberar[0] = novo, liberar[1] = monomio;
    
    novo = soma(novo, monomio);

    for(j = 0; j < 2; j++){
      libera(liberar[j]);
    }
  }
  return novo;
}

void imprime_baseln(double valor, FILE* saida){
  int exp = log10(valor);
  double coef = valor/pow(10, exp);
  if(exp < 6){
    fprintf(saida, "%f\n", valor);
    return;
  }
  fprintf(saida, "%fe+%d\n", coef, exp);
}

int main(int argc, char *argv[]){

  Polinomio pols[23];
  FILE *entrada, *saida;
  char buffer[255];

  switch(argc){
    case 1:
      entrada = stdin;
      saida = stdout;
      break;
    case 2:
      entrada = fopen(argv[1], "r");
      saida = stdout;
      break;
    case 3:
      entrada = fopen(argv[1], "r");
      saida = fopen(argv[2], "w");
      break;
  }
  if(!entrada || !saida){
    puts("Erro na abertura dos arquivos.");
    exit(1);
  }

  memset(pols, 0, sizeof(Polinomio)*23);
  while(fgets(buffer,255,entrada) != NULL){
    Polinomio* escolhido = &pols[buffer[0] - 'a'];
    switch(buffer[1]){
      case '?':{
        fprintf(saida, "%c(x) = ", buffer[0]);
        imprime(*escolhido, saida);
      }break;
      case ':':{
        libera(*escolhido);
        *escolhido = convertePolinomio(buffer + 2);      
        fprintf(saida, "%c(x) = ", buffer[0]);
        imprime(*escolhido, saida);
      }break;
      case '=':{
        *escolhido = evaluaExpressao(buffer + 2, pols);
        fprintf(saida, "%c(x) = ", buffer[0]);
        imprime(*escolhido, saida);
      }break;
      case '(':{
        double x = atof(buffer + 2);
        double valor = calcula(*escolhido, x); 
        fprintf(saida, "%c(%.2f) = ", buffer[0], x);
        imprime_baseln(valor, saida);
      }break;
    }
  }
  printf("Finalizando.");
  printf("Total de bytes: %lu", totalBytes(pols));
  fclose(entrada);
  fclose(saida);
  libera_lista();
  return 0;
}
