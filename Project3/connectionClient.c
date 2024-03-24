#include "myinet.h"
#include "errorUtils.h"
#include "connectionClient.h"

int connectionClient() {

	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *phe;
	in_addr_t serverAddress;
	char *serverName = DEFAULT_HOST;
	unsigned int serverPort = DEFAULT_PORT;

	if ((phe = gethostbyname(serverName)) != NULL)
		memcpy(&serverAddress, phe->h_addr, phe->h_length);
		else
		if ( (serverAddress = inet_addr(serverName)) == -1)
		FatalErrorSystem("Impossivel determinar endereco IP para a maquina \"%s\"",serverName);

	if ((serverPort < 1) || (serverPort > 65536)) {
		printf("O porto deve estar entre 1..65536\n");
		exit(EXIT_FAILURE);
	}

	/* Abrir um socket TCP (an Internet Stream socket) */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) /*************************/
		FatalErrorSystem("Erro ao pedir o descritor"); /*************************/

	/* Preencher a estrutura serv_addr com o endereco do servidor que pretendemos contactar */
	memset((char*) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = serverAddress;
	serv_addr.sin_port = htons(serverPort);

	printf("O cliente vai ligar-se ao servidor na maquina %s:%d\n", serverName,
			serverPort);
	printf("IP: %s\n", inet_ntoa(serv_addr.sin_addr));

	/* Ligar-se ao servidor */
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		FatalErrorSystem("Falha no connect");

	printf("Ligacao estabelecida...\n");
	return sockfd;
}
