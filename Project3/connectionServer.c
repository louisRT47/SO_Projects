#include "myinet.h"
#include "errorUtils.h"
#include "connectionServer.h"

int connectionServer() {
	int sockfd;
	struct sockaddr_in serv_addr;
	unsigned int serverPort = DEFAULT_PORT;

	signal(SIGPIPE, SIG_IGN);

	if ((serverPort < 1) || (serverPort > 65536)) {
		printf("O porto deve estar entre 1..65536\n");
		exit(EXIT_FAILURE);
	}

	printf("O Servidor vai registar-se no porto %d\n", serverPort);

	/* Criar sockect TCP */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		FatalErrorSystem("Erro ao pedir o descritor");

	/* Registar endereco local de modo a que os clientes nos possam contactar */
	memset((char*) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(serverPort);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		FatalErrorSystem("Error ao efectuar o bind");

	/* Activar socket com fila de espera de dimensao 5 */
	if (listen(sockfd, 5) < 0)
		FatalErrorSystem("Erro no listen");

	return sockfd;
}
