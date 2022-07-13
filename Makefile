CC = gcc
CFLAGS = -c -W -Wall -pedantic -lm

all: cliente

cliente: ./bin/polinomio.o ./bin/cliente.o
	$(CC) -o cliente ./bin/cliente.o ./bin/polinomio.o 

./bin/cliente.o: ./src/cliente.c ./src/polinomio.h
	$(CC) -o ./bin/cliente.o ./src/cliente.c $(CFLAGS)

./bin/polinomio.o: ./src/polinomio.c ./src/polinomio.h
	$(CC) -o ./bin/polinomio.o ./src/polinomio.c $(CFLAGS)
