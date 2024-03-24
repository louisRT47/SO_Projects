#include "myinet.h"
#include "errorUtils.h"
#include "process.h"
#include "pthread.h"
#include <semaphore.h>
#include "funcoesSocket.h"
#include "connectionServer.h"
#include "sBuffer.h"

#define N_TH_WORKERS 6
#define MAX_CAPACITY 10

typedef struct {
	int count;
	int long sizeTotal;
	pthread_mutex_t mutex;
	sem_t s1;
	sem_t s2;
	int terminarSockfd;
	int flag;
} Stat;

typedef struct {
	int sockfd;
	Stat *s;
} Args;

void LerLinhaTexto(int sockfd, char * buffer, int size) {
	int i = 0;
	int n, sum = 0;
	while ((n = read(sockfd, &buffer[i], 1)) > 0) {
		sum += n;
		if (sum > size)
			FatalErrorSystem("Exceded size of buffer");
		if (buffer[i] == '\n') {
			buffer[i] = '\0';
			break;
		}
		i++;
	}
}

int closePipe(process_t *p) {
	if (p->acessIN) {
		close(p->pipeIN[1]);
		return 0;
	}
	return -1;
}

void atenderCliente(Args *pt) {

	char filename[128];
	char size[128];
	char buffer[DIM_BUFFER + 1];

	LerLinhaTexto(pt->sockfd, filename, sizeof(filename));
	LerLinhaTexto(pt->sockfd, size, sizeof(size));
	LerLinhaTexto(pt->sockfd, buffer, sizeof(buffer));

	process_t *p;
	char *cmd[] = { "pdftotext", "-layout", "-", "-", NULL };
	int long sizeFile = atol(size);

	p = process_init_command(cmd);
	process_get_acess_stdout(p);
	process_get_acess_stdin(p);
	process_start_execution(p);

	socketReceive_server(p, pt->sockfd, buffer, sizeFile);
	if (closePipe(p) < 0)
		FatalErrorSystem("Erro ao fechar pipe");
	socketSend_server(p, pt->sockfd, buffer);

	pthread_mutex_lock(&pt->s->mutex);
	sem_wait(&pt->s->s1);
	pt->s->sizeTotal = pt->s->sizeTotal + sizeFile;
	pt->s->count = pt->s->count + 1;
	printf("\nNome do ficheiro convertido: %s\n", filename);
	printf("Tamanho ficheiro convertido: %ld\n\n", sizeFile);
	sem_post(&pt->s->s2);
	pthread_mutex_unlock(&pt->s->mutex);

	close(pt->sockfd);
}

void *workerThread(void *arg) {
	SharedBuffer *sb = (SharedBuffer*) arg;
	while (1) {
		Args *args = sharedBuffer_Get(sb);
		if (args == NULL)
			break;
		atenderCliente(args);
	}
	return NULL;
}

void *menuThread(void *arg) {
	Stat *s = (Stat*) arg;
	s->count = 0;

	int long media;

	sem_init(&s->s1, 0, 1);	//s1 iniciado a 1 de modo a que a funçao atender cliente execute primeiro o código de exclusão
	sem_init(&s->s2, 0, 0);

	while (1) {
		sem_wait(&s->s2);
		if (s->flag == 1)
			break;
		media = s->sizeTotal / s->count;
		printf("Dimensão media dos ficheiros convertidos: %ld bytes\n", media);
		printf("Dimensão total dos ficheiros convertidos: %ld bytes\n", s->sizeTotal);
		printf("Número de ficheiros convertidos: %d\n\n", s->count);
		printf("Espero ligacao...\n");
		sem_post(&s->s1);
	}
	return NULL;
}

void *terminateThread(void *arg) {
	Stat *s = (Stat*) arg;
	while (1) {
		if (getchar() == 't') {
			shutdown(s->terminarSockfd, SHUT_RDWR);
			printf("O servidor vai terminar...\n");
			break;
		}
	}
	return NULL;
}

int main(int argc, char * argv[]) {
	socklen_t dim_client;
	struct sockaddr_in client_addr;
	int newsockfd;
	int sockfd;

	if (argc == 1)
		sockfd = connectionServer();
	else {
		printf("Argumentos invalidos.\nUse: %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	Stat s;
	s.sizeTotal = 0;
	s.count = 0;
	s.flag = 0;
	s.terminarSockfd = sockfd;

	pthread_t th_menu;
	if (pthread_create(&th_menu, NULL, menuThread, &s)!= 0)
		FatalErrorSystem("Erro na thread menu");

	SharedBuffer sb;

	pthread_t th_worker[N_TH_WORKERS];
	int i;
	for (i = 0; i < N_TH_WORKERS; i++)
		if (pthread_create(&th_worker[i], NULL, workerThread, &sb) != 0)
			FatalErrorSystem("Erro na thread worker");

	pthread_t th_terminate;
	if (pthread_create(&th_terminate, NULL, terminateThread, &s) != 0)
		FatalErrorSystem("Erro na thread terminate");

	sharedBuffer_init(&sb, MAX_CAPACITY);
	pthread_mutex_init(&s.mutex, NULL);

	printf("Espero ligacao...\n");

	for (;;) {
		dim_client = sizeof(client_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) (&client_addr), &dim_client);

		if (newsockfd < 0) {
			if (errno == EINVAL)
				break;
			FatalErrorSystem("Erro ao efectuar o accept");
		}

		printf("Estabeleci uma ligacao...\n");

		Args *p = malloc(sizeof(Args));
		p->s = &s;
		p->sockfd = newsockfd;

		sharedBuffer_Put(&sb, p);
	} //for

	for (i = 0; i < N_TH_WORKERS; i++)
		sharedBuffer_Put(&sb, NULL);//mandar NULL para as threads workers terminarem

	for (i = 0; i < N_TH_WORKERS; i++)
		if (pthread_join(th_worker[i], NULL) != 0)
			FatalErrorSystem("Erro a terminar thread worker");

	s.flag = 1;	//ativar flag para terminar thread menu

	sem_post(&s.s2);	//desbloquear thread menu

	if (pthread_join(th_menu, NULL) != 0)
		FatalErrorSystem("Erro a terminar thread menu");
	if (pthread_join(th_terminate, NULL) != 0)
		FatalErrorSystem("Erro a terminar thread terminate");

	sharedBuffer_destroy(&sb);
	close(sockfd);
	printf("O servidor terminou...\n");
	return 0;
}
