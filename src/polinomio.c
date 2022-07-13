#include "polinomio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct termo{
  double coef;
  int exp;
  struct termo *prox;
}Termo;

size_t bytes_alocados = 0;

static Termo *lista_livre = NULL;

static Termo *aloca_termo(){
  Termo* novo;
  if(lista_livre == NULL){
    //Caso nenhum elemento esteja livre.
    bytes_alocados += sizeof(Termo);
    novo = malloc(sizeof(Termo));
  }else{
    //Caso exista elementos ja alocados na lista livre.
    novo = lista_livre;
    lista_livre = lista_livre->prox;
    novo->prox = NULL;
  }
  printf("Bytes alocados: %lu\n", bytes_alocados);
  return novo;
}
static void libera_termo(Termo *p){
  //Resetando os valores.
  p->coef = 0;
  p->exp = 0;
  //Inserindo no inicio.
  p->prox = lista_livre;
  lista_livre = p;
  printf("Bytes alocados: %lu\n", bytes_alocados);
}

void libera_lista(){
  printf("Bytes alocados: %lu\n", bytes_alocados);
  while(lista_livre){
    Termo* liberar = lista_livre;
    lista_livre = lista_livre->prox;
    free(liberar);
  }
}

static Polinomio multiplica_por_monomio(Polinomio p, double coef, int exp){

  Polinomio head = cria_monomio(p->coef * coef, p->exp + exp);
  Polinomio ptr = head;

  p = p->prox;
  
  while(p){
    ptr->prox = cria_monomio(p->coef * coef, p->exp + exp); 
    ptr = ptr->prox;
    p = p->prox;
  }

  return head;
}

//HANDLERS
static void inverte_sinal(Polinomio p){
  while(p){
    p->coef *= -1;
    p = p->prox;
  }
}
static int compara_termo(Termo* p, Termo* q){
  return p->exp - q->exp;
}

//INTERFACE
Polinomio cria_monomio(double coef, int exp){
  if(coef == 0){
    return NULL;
  }
  Polinomio novo = aloca_termo();
  novo->exp = exp;
  novo->coef = coef;
  novo->prox = NULL;
  return novo;
}

Polinomio soma(Polinomio p, Polinomio q){

  if(!p || !q){
    if(p)
      return copia(p);
    
    if(q)
      return copia(q);

    return NULL;
  }

  Polinomio head;
  Polinomio ptr;

  if(compara_termo(p, q) > 0){
    head = cria_monomio(p->coef, p->exp);
    p = p->prox;
  }else if(compara_termo(p, q) < 0){
    head = cria_monomio(q->coef, q->exp);
    q = q->prox;
  }else{
    head = cria_monomio(p->coef + q->coef, q->exp);
    p = p->prox;
    q = q->prox;
  }

  ptr = head;

  while(p && q){

    while(p && q && compara_termo(p, q) > 0){
      ptr->prox = cria_monomio(p->coef, p->exp);
      ptr = ptr->prox;
      p = p->prox;
    }
    while(p && q && compara_termo(p, q) < 0){
      ptr->prox = cria_monomio(q->coef, q->exp);
      ptr = ptr->prox;
      q = q->prox;
    }

    if(p && q && compara_termo(p, q) == 0){
      ptr->prox = cria_monomio(p->coef + q->coef, q->exp);
      ptr = ptr->prox;
      p = p->prox;
      q = q->prox;
    }
  }

  if(p)
    ptr->prox = copia(p); 
  if(q)
    ptr->prox = copia(q);
  

  return head;
}

Polinomio subtrai(Polinomio p, Polinomio q){

  if(!p || !q){
    if(p)
      return copia(p);
    
    if(q)
      return copia(q);

    return NULL;
  }

  Polinomio head;
  Polinomio ptr;

  if(compara_termo(p, q) > 0){
    head = cria_monomio(p->coef, p->exp);
    p = p->prox;
  }else if(compara_termo(p, q) < 0){
    head = cria_monomio(q->coef * -1, q->exp);
    q = q->prox;
  }else{
    head = cria_monomio(p->coef - q->coef, q->exp);
    p = p->prox;
    q = q->prox;
  }

  ptr = head;

  while(p && q){

    while(p && q && compara_termo(p, q) > 0){
      ptr->prox = cria_monomio(p->coef, p->exp);
      ptr = ptr->prox;
      p = p->prox;
    }
    while(p && q && compara_termo(p, q) < 0){
      ptr->prox = cria_monomio(q->coef * -1, q->exp);
      ptr = ptr->prox;
      q = q->prox;
    }

    if(p && q && compara_termo(p, q) == 0){
      ptr->prox = cria_monomio(p->coef - q->coef, q->exp);
      ptr = ptr->prox;
      p = p->prox;
      q = q->prox;
    }
  }

  if(p)
    ptr->prox = copia(p); 
  if(q)
    while(q){
      ptr->prox =cria_monomio(q->coef * -1, q->exp); 
      ptr = ptr->prox;
      q = q->prox;
    }

  return head;
}

Polinomio multiplica(Polinomio p, Polinomio q){
  if(!p || !q){
    if(p)
      return copia(p);
    
    if(q)
      return copia(q);

    return NULL;
  }

  Polinomio head = NULL;
  Polinomio temp = NULL;
  Polinomio multiplicado = NULL;

  while(q){

    temp = copia(p);
    multiplicado = multiplica_por_monomio(temp, q->coef, q->exp);
    
    Polinomio liberar[] = {head, temp, multiplicado};

    head = soma(head, multiplicado);

    for(int i = 0; i < 3; i++){
      libera(liberar[i]);
    }

    q = q->prox;
  }

  return head;
}

Polinomio oposto(Polinomio p){
  Polinomio head = cria_monomio(p->coef * -1, p->exp);
  Polinomio ptr = head;
  p = p->prox;
  while(p){
    ptr->prox = cria_monomio(p->coef * -1, p->exp);
    ptr = ptr->prox;
    p = p->prox;
  }

  return head;
}

Polinomio copia(Polinomio p){
  if(p == NULL){
    return NULL;
  }
  Polinomio inicio = aloca_termo(); 
  inicio->coef = p->coef;
  inicio->exp = p->exp;
  
  p = p->prox;

  Polinomio ponteiro = inicio;

  while(p != NULL){

    ponteiro->prox = aloca_termo();
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
      libera_termo(libera);
    }
  }
}

