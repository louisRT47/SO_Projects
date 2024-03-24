#include "funcoesSocket.h"
#include "errorUtils.h"

void socketSend_client(int fd, char *buf, int dim, int sockfd) {
	int n_bytes;
	while ((n_bytes = read(fd, buf, dim)) > 0) {
		if (n_bytes < 0)
			FatalErrorSystem("Erro no read");
		if (write(sockfd, buf, n_bytes) < 0)
			FatalErrorSystem("Erro no write do client");
	}
}

void socketReceive_client(int fd, char *buf, int dim, int sockfd) {
	int n_bytes;
	while ((n_bytes = read(sockfd, buf, dim)) > 0) {
		if (n_bytes < 0)
			FatalErrorSystem("Erro no read");
		if (write(fd, buf, n_bytes) < 0)
			FatalErrorSystem("Erro no write");
	}
}

void socketReceive_server(process_t *p, int sockfd, char *buf, int size) {
	int sum = 0, n_bytes;
	while ((n_bytes = read(sockfd, buf, sizeof(buf))) != 0) {
		if (n_bytes < 0)
			FatalErrorSystem("Erro no read");
		if (process_write_in(p, buf, n_bytes) < 0)
			FatalErrorSystem("Erro no process write");
		sum += n_bytes;
		if (sum == size) {
			break;
		}
	}
}

void socketSend_server(process_t *p, int sockfd, char *buf) {
	int n_bytes;
	while ((n_bytes = process_read_out(p, buf, sizeof(buf))) > 0) {
		if (n_bytes < 0)
			FatalErrorSystem("Erro no process read");
		if (write(sockfd, buf, n_bytes) < 0)
			FatalErrorSystem("Erro no write do server");
	}
}

