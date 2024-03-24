all: maintest

maintest: maintest.o process.o
	gcc -g -Wall -o maintest maintest.o process.o 

maintest.o: maintest.c process.h
	gcc -g -c -Wall maintest.c

process.o: process.c process.h
	gcc -g -c -Wall process.c

clean:
	rm -f maintest
	rm -f 01-Trabalho.txt
	rm -f *.o
	