#include "polinomio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct termo{
  double coef;
  int exp;
  struct termo *prox;
}Termo;

static Termo *lista_livre = NULL;

//HANDLERS
Polinomio adiciona_termo(Polinomio lista, Polinomio termo){
  if(lista == NULL){
    return NULL;
  }
  Polinomio ponteiro = lista;
  Polinomio proximo = ponteiro->prox;
  while(proximo != NULL && termo->exp < proximo->exp){
    ponteiro = proximo;
    proximo = ponteiro->prox;
  }
  
  if(proximo != NULL && proximo->exp == termo->exp){
    proximo->coef += termo->coef;
  }else if(ponteiro != NULL && ponteiro->exp == termo->exp){
    ponteiro->coef += termo->coef;
  }else{
    ponteiro->prox = copia(termo);
    ponteiro->prox->prox = proximo;
  }

  return lista;
}
//INTERFACE
Polinomio cria_monomio(double coef, int exp){
  if(coef == 0){
    return NULL;
  }
  Polinomio novo = malloc(sizeof(Termo));
  novo->exp = exp;
  novo->coef = coef;
  novo->prox = NULL;
  return novo;
}

Polinomio soma(Polinomio p, Polinomio q){

  if(!p || !q){
    if(p)
      return p;
    
    if(q)
      return q;

    return NULL;
  }

  Polinomio head;
  Polinomio ptr;

  if(p->exp < q->exp){
    head = cria_monomio(p->coef, p->exp);
  }else if(p->exp > q->exp){
    head = cria_monomio(q->coef, q->exp);
  }else{
    head = cria_monomio(q->coef + p->coef, q->exp);
  }
  
  ptr = head;


  return head;
}

Polinomio copia(Polinomio p){
  if(p == NULL){
    return NULL;
  }
  Polinomio inicio = malloc(sizeof(Termo)); 
  inicio->coef = p->coef;
  inicio->exp = p->exp;
  
  p = p->prox;

  Polinomio ponteiro = inicio;

  while(p != NULL){

    ponteiro->prox = malloc(sizeof(Termo));
    ponteiro->prox->coef = p->coef;
    ponteiro->prox->exp = p->exp;

    ponteiro = ponteiro->prox;
    p = p->prox;
  }

  return inicio;
}

void imprime(Polinomio p, FILE *arq){
  if(p == NULL){
    printf("Nulo.");
  }

  for(int i = 0; p != NULL; i++){
    if(p->prox){
      if(p->coef > 0){
        if(i == 0){
          fprintf(arq, "%.0lfx^%d ", p->coef, p->exp);
        }else{
          fprintf(arq, "+%.0lfx^%d ", p->coef, p->exp);
        }
      }else{
        fprintf(arq, "%.0lfx^%d ", p->coef, p->exp);
      }
    }else{
      if(p->coef > 0){
        fprintf(arq, "+%.0lfx^%d\n", p->coef, p->exp);
      }else{
        fprintf(arq, "%.0lfx^%d\n", p->coef, p->exp);
      }
    }
    p = p->prox;
  }
}
    
void libera(Polinomio p){
  if(p != NULL){
    while(p != NULL){
      Polinomio libera = p;
      p = p->prox;
      free(libera);
    }
  }
}

