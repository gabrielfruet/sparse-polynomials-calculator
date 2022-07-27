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
    //Caso nenhum elemento esteja livre.
    bytes_alocados += sizeof(Termo);
    novo = malloc(sizeof(Termo));
  }else{
    //Caso exista elementos ja alocados na lista livre.
    novo = lista_livre;
    lista_livre = lista_livre->prox;
    novo->prox = NULL;
  }
  /*printf("ALOCANDO\nBytes alocados: %lu\n", bytes_alocados);*/
  return novo;
}
static void libera_termo(Termo *p){
  //Resetando os valores.
  p->coef = 0;
  p->exp = 0;
  //Inserindo no inicio.
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

//HANDLERS
static void inverte_sinal(Polinomio p){
  while(p){
    p->coef *= -1;
    p = p->prox;
  }
}
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
  for(int i = 0; i < n; i++){
    libera(v[i]);
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

static Polinomio soma_ou_subtrai(Polinomio p, Polinomio q, int operacao){
  if(!p || !q){
    if(p)
      return copia(p);
    
    if(q)
      return copia(q);

    return NULL;
  }

  Polinomio head = NULL;
  Polinomio ptr = NULL;

  while(head == NULL && (p || q)){
    if(compara_termo_exp(p, q) > 0){
      head = cria_monomio(p->coef, p->exp);
      p = p->prox;
    }else if(compara_termo_exp(p, q) < 0){
      head = cria_monomio(q->coef * operacao, q->exp);
      q = q->prox;
    }else if(compara_termo_exp(p, q) == 0){
      head = cria_monomio(p->coef - q->coef, q->exp);
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
      ptr->prox = cria_monomio(p->coef - q->coef, q->exp);
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
  if(t == NULL || t->exp == 0){
    return NULL;
  }

  Termo* derivado = aloca_termo();
  derivado->coef = t->coef * t->exp;
  derivado->exp = t->exp - 1;

  return derivado;
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
  return soma_ou_subtrai(p, q, SOMA);
}

Polinomio subtrai(Polinomio p, Polinomio q){
  return soma_ou_subtrai(p, q, SUBTRAI);
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

    libera_polinomios(liberar, 3);

    q = q->prox;
  }

  return head;
}
Polinomio divide(Polinomio p, Polinomio q){
  if(!p || !q){
    return NULL;
  }

  Polinomio dividendo = copia(p);
  Polinomio divisor = q;
  Polinomio quociente = NULL;
  Polinomio subtrair = NULL;
  Polinomio adiciona_quociente = NULL;
  int coef, exp;

  do{
    coef = dividendo->coef/divisor->coef;
    exp = dividendo->exp - divisor->exp;

    if(exp >= 0){

      adiciona_quociente = cria_monomio(coef, exp);
      quociente = soma(quociente, adiciona_quociente);

      subtrair = multiplica_por_monomio(divisor, coef, exp);
      Polinomio liberar[] = { subtrair, dividendo, adiciona_quociente};
      dividendo = subtrai(dividendo, subtrair);

      libera_polinomios(liberar, 3);
    }
  }while(dividendo && exp >= 0);

  return quociente;
}

Polinomio resto(Polinomio p, Polinomio q){
  if(!p || !q){
    return NULL;
  }

  Polinomio resto = copia(p);
  Polinomio divisor = q;
  Polinomio subtrair = NULL;
  int coef, exp;

  do{
    coef = resto->coef/divisor->coef;
    exp = resto->exp - divisor->exp;

    if(exp >= 0){
      subtrair = multiplica_por_monomio(divisor, coef, exp);
      Polinomio liberar[] = { subtrair, resto};
      resto = subtrai(resto, subtrair);

      libera_polinomios(liberar, 2);
    }
  }while(resto && exp >= 0);

  return resto;
}

Polinomio deriva(Polinomio p){
  if(!p){
    return NULL;
  }

  Termo* termo = deriva_termo(p);
  if(!termo){
    return NULL;
  }

  Polinomio head = termo;
  Polinomio ptr = head;
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
  if(p == NULL){
    puts("0");
    return;
  }

  for(int i = 0; p != NULL; i++){

    if(p->coef == 1 && p->exp != 0 && i != 0){
      fprintf(arq, "+ "); 
    }else if(p->coef == -1 && p->exp != 0){
      fprintf(arq, "- "); 
    }else{
      if(p->coef > 0 && i != 0){
        fprintf(arq, "+ ");
        fprintf(arq, "%.0lf", p->coef);
      }else if(p->coef < 0){
        fprintf(arq, "- ");
        fprintf(arq, "%.0lf", p->coef * -1);
      }else{
        fprintf(arq, "%.0lf", p->coef);
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

