#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "calculadora.h"
#include "polinomio.h"
#define POSF_SIZE 100
#define OPERANDO 1
#define OPERADOR 0

struct pilha_operador{
  char *vetor;
  int s;
  int c;
};

typedef struct pilha_operador* Pilha;

Pilha cria_pilha(){
  Pilha p = malloc(sizeof(struct pilha_operador));
  p->s = 0;
  p->c = 100;
  p->vetor = malloc(sizeof(char)*p->c);
  memset(p->vetor, 0, sizeof(char)*100);
  return p;
}

void libera_pilha(Pilha p){
  free(p->vetor);
  free(p);
}

int vazia(Pilha p){
  return p->s == 0;
}

int cheia(Pilha p){
  return p->s >= p->c;
}

char topo(Pilha p){
  if(vazia(p)){
    return 0;
  }
  return p->vetor[p->s - 1];
}

char empilha(Pilha p, char c){
  if(cheia(p)){
    return 0;
  }
  p->vetor[p->s++] = c;
  return c;
}
char desempilha(Pilha p){
  if(vazia(p)){
    return 0;
  }
  return p->vetor[--p->s];
}

char operador[] = "+-*/%^~'";

int eOperador(char op){
  char* resp = strchr(operador, op);
  return resp != NULL;
}

int eOperadorUnico(char op){
  char* resp = strchr("'~", op);
  return resp != NULL; 
}

int eOperando(char op){
  return isalpha(op);
}

int prioridade(char op){

  switch(op){
    case '+':
      return 1;
    case '-':
      return 1;
    case '*':
      return 2;
    case '/':
      return 2;
    case '%':
      return 2;
    case '~':
      return 3;
    case '\'':
      return 4;
  }
  return -1;

}

char* infixoParaPosfixo(char* expr){
  int i;
  Pilha p = cria_pilha();
  Pilha posf = cria_pilha();
  char* out;
  
  for(i = 0; expr[i]; ++i){

    if(eOperando(expr[i])){

      empilha(posf, expr[i]);

    }else if(expr[i] == '('){

      empilha(p, expr[i]);

    }else if(expr[i] == ')'){

      while(!vazia(p) && topo(p) != '('){
        empilha(posf, desempilha(p));
      }

      if(!vazia(p) && topo(p) != '('){
        return NULL;
      }else{
        desempilha(p);
      }

    }else{

      while(!vazia(p) && prioridade(expr[i]) <= prioridade(topo(p))){
        empilha(posf, desempilha(p));
      }
      empilha(p, expr[i]);
      
    }
  }
  while(!vazia(p)){
    empilha(posf, desempilha(p));
  }

  empilha(posf, expr[i]);
  /*printf("%s\n", posf->vetor); */
  libera_pilha(p);
  out = posf->vetor;
  free(posf);
  return out;
}

Polinomio evaluaExpressao(char* infix, Polinomio pols[23]){
  char* posfix = infixoParaPosfixo(infix);
  int i, tamanho;
  Polinomio pilha[100];

  for(i = 0; i < 100; i++)
    pilha[i] = NULL;

  tamanho = 0;

  for(i = 0; posfix[i] && posfix[i] != '\n'; i++){
    char ch = posfix[i];

    if(eOperador(ch)){

      if(eOperadorUnico(ch)){
        Polinomio a = NULL, b = NULL;
        a = pilha[--tamanho];

        switch(ch){
          case '\'':
            b = deriva(a);
            break;
          case '~':
            b = oposto(a);
            break;
        }
        libera(a);
        pilha[tamanho++] = b;
      }else{
        Polinomio a = NULL, b = NULL, c = NULL;
        b = pilha[--tamanho];
        a = pilha[--tamanho];

        switch (ch) {
          case '+':
            c = soma(a,b);
            break;
          case '-':
            c = subtrai(a, b);
            break;
          case '*':
            c = multiplica(a, b);
            break;
          case '/':
            c = divide(a,b);
            break;
          case '%':
            c = resto(a,b);
            break;
        }
        pilha[tamanho++] = c;
      }

    }else{
      pilha[tamanho++] = copia(pols[tolower(ch) - 'a']);
    }
  }
  free(posfix);
  return pilha[0];
}
