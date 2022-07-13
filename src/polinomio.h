#ifndef __POLINOMIO_H__
#include <stdio.h>

typedef struct termo *Polinomio;
//Cria um novo monômio (um polinômio com um só termo) cujo coeficiente é coef e cujo expoente é exp e devolve o monômio criado. Caso o parâmetro coef seja igual a zero, esta função cria um polinômio nulo (um polinômio que não possui nenhum termo com coeficiente não nulo) e devolve o polinômio nulo criado.
Polinomio cria_monomio(double coef, int exp);
//Recebe dois polinômios, p e q, e devolve como valor da função a soma de p e q.
Polinomio soma(Polinomio p, Polinomio q);
//Recebe dois polinômios, p e q, e devolve como valor da função a diferença entre p e q, ou seja, p-q.
Polinomio subtrai(Polinomio p, Polinomio q);
//Recebe dois polinômios, p e q, e devolve como valor da função o produto de p e q.
Polinomio multiplica(Polinomio p, Polinomio q);
//Recebe dois polinômios, p e q, e devolve como valor da função o quociente da divisão de p por q. Veja o exemplo no item abaixo.
Polinomio divide(Polinomio p, Polinomio q);
//Recebe dois polinômios, p e q, e devolve como valor da função o resto da divisão de p por q. 
//Exemplo: Considere os polinômios p(x)=-8x5+2x3+5x2-7 e q(x)=2x3-x2+2. O quociente da divisão de p(x) por q(x) é o polinômio t(x)=-4x2-2x. O resto dessa divisão é o polinômio r(x)=13x2+4x-7. De fato, o polinômio r(x) tem grau estritamente menor que o grau de q(x) e é fácil verificar que p(x)=q(x)t(x)+r(x). 
Polinomio resto(Polinomio p, Polinomio q);
//Recebe um polinômio p e devolve como valor da função o oposto de p. 
//Exemplo: O oposto do polinômio p(x)=-8x5+2x3+5x2-7 é o polinômio -p(x)=8x5-2x3-5x2+7.
Polinomio oposto(Polinomio p);
//Recebe um polinômio p e devolve como valor da função a derivada de p.
Polinomio deriva(Polinomio p);
//Recebe um polinômio p e devolve como valor da função uma cópia de p.
Polinomio copia(Polinomio p);
//Recebe um polinômio p e devolve como valor da função o grau de p.
int grau(Polinomio p);
//Recebe um polinômio p e um real x e devolve o valor do polinômio p em x.
double calcula(Polinomio p, double x);
//Recebe um polinômio p e o imprime no arquivo arq. O parâmetro arq deve ser um arquivo aberto para escrita.
void imprime(Polinomio p, FILE *arq);
//Libera a memória ocupada pelo polinômio p.
void libera(Polinomio p);

#define __POLINOMIO_H__
#endif
