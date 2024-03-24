all: server client

server: serverTCP.o process.o errorUtils.o funcoesSocket.o connectionServer.o sBuffer.o
	gcc -g -Wall -pthread serverTCP.o process.o errorUtils.o funcoesSocket.o connectionServer.o sBuffer.o -o server

client:	clienteTCP.o errorUtils.o funcoesSocket.o connectionClient.o
	gcc -g -Wall -pthread clienteTCP.c errorUtils.o funcoesSocket.o connectionClient.o process.o -o client

serverTCP.o: serverTCP.c process.h myinet.h errorUtils.h funcoesSocket.h connectionServer.h sBuffer.h
	gcc -g -c -Wall -pthread serverTCP.c 

clientTCP.o: clienteTCP.c myinet.h errorUtils.h funcoesSocket.h connectionClient.h
	gcc -g -c -Wall -pthread clienteTCP.c 

errorUtils.o: errorUtils.c errorUtils.h
	gcc -g -c -Wall errorUtils.c

funcoesSocket.o: funcoesSocket.c funcoesSocket.h process.h errorUtils.h
	gcc -g -c -Wall funcoesSocket.c 
	
connectionClient.o: connectionClient.c connectionClient.h errorUtils.h myinet.h
	gcc -g -c -Wall connectionClient.c 
	
connectionServer.o: connectionServer.c connectionServer.h errorUtils.h myinet.h
	gcc -g -c -Wall connectionServer.c 
	
sBuffer.o: sBuffer.c sBuffer.h
	gcc -g -c -Wall sBuffer.c
	
process.o: process.c process.h
	gcc -g -c -Wall process.c

clean:
	rm -f server client
	rm -f *.o
	rm -f *.txt