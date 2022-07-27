#include "polinomio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define SOMA 1
#define SUBTRAI -1


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
    bytes_alocados += sizeof(Termo);
    novo = malloc(sizeof(Termo));
  }else{
    novo = lista_livre;
    lista_livre = lista_livre->prox;
    novo->prox = NULL;
  }
  /*printf("ALOCANDO\nBytes alocados: %lu\n", bytes_alocados);*/
  return novo;
}
static void libera_termo(Termo *p){
  p->coef = 0;
  p->exp = 0;
  p->prox = lista_livre;
  lista_livre = p;
  /*printf("LIBERANDO\nBytes alocados: %lu\n", bytes_alocados);*/
}

void libera_lista(){
  while(lista_livre){
    Termo* liberar = lista_livre;
    lista_livre = lista_livre->prox;
    free(liberar);
  }
}

/*static void imprime_polinomio_debug(Polinomio p){*/
  /*while(p->prox){*/
    /*printf("{ coef = %f, exp = %d, prox = %p } -> \n", p->coef, p->exp, p->prox);*/
    /*p = p->prox;*/
  /*}*/
  /*printf("NULL\n");*/
/*}*/

/*HANDLERS*/
static int compara_termo_exp(Termo* p, Termo* q){
  if(!p){
    return q->exp;
  }
  if(!q){
    return -1*p->exp;
  }
  return p->exp - q->exp;
}

static void libera_polinomios(Polinomio v[], int n){
  int i;
  for(i = 0; i < n; i++){
    libera(v[i]);
  }
}
static Polinomio multiplica_por_monomio(Polinomio p, double coef, int exp){
  Polinomio head,ptr;
  if(coef == 0.0){
    return NULL;
  }

  head = cria_monomio(p->coef * coef, p->exp + exp);
  ptr = head;

  p = p->prox;
  
  while(p){
    ptr->prox = cria_monomio(p->coef * coef, p->exp + exp); 
    ptr = ptr->prox;
    p = p->prox;
  }

  return head;
}

static Polinomio soma_ou_subtrai(Polinomio p, Polinomio q, int operacao){
  Polinomio head,ptr;
  if(!p || !q){
    if(p)
      return copia(p);
    
    if(q)
      return copia(q);

    return NULL;
  }

  head = NULL;
  ptr = NULL;

  while(head == NULL && (p || q)){
    if(compara_termo_exp(p, q) > 0){
      head = cria_monomio(p->coef, p->exp);
      p = p->prox;
    }else if(compara_termo_exp(p, q) < 0){
      head = cria_monomio(q->coef * operacao, q->exp);
      q = q->prox;
    }else if(compara_termo_exp(p, q) == 0){
      head = cria_monomio(p->coef + (operacao*q->coef), q->exp);
      p = p->prox;
      q = q->prox;
    }
  }

  ptr = head;

  while(p && q){
    while(p && q && compara_termo_exp(p, q) > 0){
      ptr->prox = cria_monomio(p->coef, p->exp);
      ptr = ptr->prox;
      p = p->prox;
    }
    while(p && q && compara_termo_exp(p, q) < 0){
      ptr->prox = cria_monomio(q->coef * operacao, q->exp);
      ptr = ptr->prox;
      q = q->prox;
    }
    if(p && q && compara_termo_exp(p, q) == 0){
      ptr->prox = cria_monomio(p->coef + (operacao*q->coef), q->exp);
      if(ptr->prox != NULL){
        ptr = ptr->prox;
      }
      p = p->prox;
      q = q->prox;
    }
  }

  if(p)
    ptr->prox = copia(p); 
  if(q)
    while(q){
      ptr->prox =cria_monomio(q->coef * operacao, q->exp); 
      ptr = ptr->prox;
      q = q->prox;
    }

  return head;
}

Termo* deriva_termo(Termo* t){
  Termo* derivado;
  if(t == NULL || t->exp == 0){
    return NULL;
  }

  derivado = aloca_termo();
  derivado->coef = t->coef * t->exp;
  derivado->exp = t->exp - 1;

  return derivado;
}

/*INTERFACE*/
Polinomio cria_monomio(double coef, int exp){
  Polinomio novo;
  if(coef == 0){
    return NULL;
  }
  novo = aloca_termo();
  novo->exp = exp;
  novo->coef = coef;
  novo->prox = NULL;
  return novo;
}

Polinomio soma(Polinomio p, Polinomio q){
  return soma_ou_subtrai(p, q, SOMA);
}

Polinomio subtrai(Polinomio p, Polinomio q){
  return soma_ou_subtrai(p, q, SUBTRAI);
}

Polinomio multiplica(Polinomio p, Polinomio q){
  Polinomio head = NULL, temp = NULL, multiplicado = NULL;

  if(!p || !q){
    if(p)
      return copia(p);
    
    if(q)
      return copia(q);

    return NULL;
  }

  while(q){
    Polinomio liberar[3];

    temp = copia(p);
    multiplicado = multiplica_por_monomio(temp, q->coef, q->exp);
    
    liberar[0] = head, liberar[1] = temp, liberar[2] = multiplicado;

    head = soma(head, multiplicado);

    libera_polinomios(liberar, 3);

    q = q->prox;
  }

  return head;
}
Polinomio divide(Polinomio p, Polinomio q){
  Polinomio dividendo, divisor, quociente, subtrair, adiciona_quociente;
  double coef;
  int exp;

  if(!p || !q){
    return NULL;
  }

  dividendo = copia(p);
  divisor = q;
  quociente = NULL, subtrair = NULL, adiciona_quociente = NULL;

  do{
    coef = dividendo->coef/divisor->coef;
    exp = dividendo->exp - divisor->exp;

    if(exp >= 0){
      Polinomio liberar[3];

      adiciona_quociente = cria_monomio(coef, exp);
      quociente = soma(quociente, adiciona_quociente);

      subtrair = multiplica_por_monomio(divisor, coef, exp);
      liberar[0] = subtrair, liberar[1] = dividendo, liberar[2] = adiciona_quociente;
      dividendo = subtrai(dividendo, subtrair);

      libera_polinomios(liberar, 3);
    }
  }while(dividendo && exp >= 0);

  return quociente;
}

Polinomio resto(Polinomio p, Polinomio q){
  Polinomio resto, divisor, subtrair;
  double coef;
  int exp;

  if(!p || !q){
    return NULL;
  }

  resto = copia(p);
  divisor = q;
  subtrair = NULL;

  do{
    coef = resto->coef/divisor->coef;
    exp = resto->exp - divisor->exp;

    if(exp >= 0){
      Polinomio liberar[2];

      subtrair = multiplica_por_monomio(divisor, coef, exp);
      liberar[0] = subtrair, liberar[1] = resto;

      resto = subtrai(resto, subtrair);

      libera_polinomios(liberar, 2);
    }
  }while(resto && exp >= 0);

  return resto;
}

Polinomio deriva(Polinomio p){
  Termo* termo;
  Polinomio head,ptr;

  if(!p){
    return NULL;
  }

  termo = deriva_termo(p);

  if(!termo){
    return NULL;
  }

  head = termo;
  ptr = head;
  p = p->prox;
  while(p){

    termo = deriva_termo(p);

    if(termo){
      ptr->prox = termo;
      ptr = ptr->prox;
    }
    p = p->prox;
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
  Polinomio inicio, ponteiro;

  if(p == NULL){
    return NULL;
  }
  inicio = aloca_termo(); 
  inicio->coef = p->coef;
  inicio->exp = p->exp;
  
  p = p->prox;

  ponteiro = inicio;

  while(p != NULL){

    ponteiro->prox = aloca_termo();
    ponteiro->prox->coef = p->coef;
    ponteiro->prox->exp = p->exp;

    ponteiro = ponteiro->prox;
    p = p->prox;
  }

  return inicio;
}

int grau(Polinomio p){
  return p ? p->exp : 0;
}

double calcula(Polinomio p, double x){
  double resultado = 0;
  while(p){
    resultado += p->coef*pow(x, p->exp);
    p = p->prox;
  }
  return resultado;
}

void imprime(Polinomio p, FILE *arq){
  int i;
  if(p == NULL){
    puts("0");
    return;
  }

  for(i = 0; p != NULL; i++){

    if(p->coef == 1 && p->exp != 0 && i != 0){
      fprintf(arq, "+ "); 
    }else if(p->coef == -1 && p->exp != 0){
      fprintf(arq, "- "); 
    }else{
      if(p->coef > 0 && i != 0){
        fprintf(arq, "+ ");
        fprintf(arq, "%.0f", p->coef);
      }else if(p->coef < 0){
        fprintf(arq, "- ");
        fprintf(arq, "%.0f", p->coef * -1);
      }else{
        fprintf(arq, "%.0f", p->coef);
      }
    }
    if(p->exp == 1) {
      fprintf(arq, "x");
    }else if(p->exp){
      fprintf(arq, "x^%d", p->exp);
    }

    if(p->prox){
      fprintf(arq, " ");
    }else{
      fprintf(arq, "\n");
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

