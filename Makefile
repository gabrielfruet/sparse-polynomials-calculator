CC = gcc
CFLAGS = -c -W -Wall -pedantic -lm -ansi -U_FORTIFY_SOURCE

all: cliente

clear: bin/
	rm -rf bin/*.o

cliente: ./bin/ ./bin/polinomio.o ./bin/cliente.o ./bin/calculadora.o
	$(CC) -o cliente ./bin/cliente.o ./bin/polinomio.o ./bin/calculadora.o -lm

./bin/cliente.o: ./src/cliente.c ./src/polinomio.h
	$(CC) -o ./bin/cliente.o ./src/cliente.c $(CFLAGS)

./bin/polinomio.o: ./src/polinomio.c ./src/polinomio.h
	$(CC) -o ./bin/polinomio.o ./src/polinomio.c $(CFLAGS)

./bin/calculadora.o: ./src/calculadora.c ./src/calculadora.h
	$(CC) -o ./bin/calculadora.o ./src/calculadora.c $(CFLAGS)

./bin/:
	mkdir bin
