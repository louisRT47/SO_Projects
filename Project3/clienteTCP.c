#include "myinet.h"
#include "errorUtils.h"
#include "funcoesSocket.h"
#include "connectionClient.h"
#include "pthread.h"

typedef struct {
	char filename[30];
	int cnt;
	pthread_mutex_t mutex;
} File;

void *thClient(void *arg) {
	File *f = ((File*) arg);
	char buffer[DIM_BUFFER + 1];

	int sockfd = connectionClient();

	char filename[30];
	strcpy(filename, f->filename);

	int file;
	file = open(filename, O_RDONLY, 0644);
	if (file < 0)
		FatalErrorSystem("Erro a abrir ficheiro");
	write(sockfd, filename, strlen(filename));

	struct stat fileStat;
	fstat(file, &fileStat);
	long int size = fileStat.st_size;
	sprintf(buffer, "\n%ld\n\n", size);
	write(sockfd, buffer, strlen(buffer));

	socketSend_client(file, buffer, DIM_BUFFER, sockfd);
	close(file);

	int a = strlen(filename);
	int i = a - 5;
	while (a != i) {
		filename[a] = '\0';
		a--;
	}

	pthread_mutex_lock(&f->mutex);
	sprintf(filename, "%s%d.txt", filename, f->cnt);
	f->cnt++;
	pthread_mutex_unlock(&f->mutex);

	int newFile = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (newFile < 0)
		FatalErrorSystem("Erro a criar ficheiro");

	socketReceive_client(newFile, buffer, DIM_BUFFER, sockfd);
	printf("Recebi %s\n", filename);
	close(newFile);

	close(sockfd);
	return NULL;
}

int main(int argc, char * argv[]) {

	if (argc != 3) {
		printf("Argumentos invalidos.\nUse: %s <filename.pdf> <numero de clientes>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	File f;
	f.cnt = 0;

	strcpy(f.filename, argv[1]);
	int nClients = atoi(argv[2]);

	pthread_mutex_init(&f.mutex, NULL);

	pthread_t th[nClients];
	int i;
	for (i = 0; i < nClients; i++)
		if (pthread_create(&th[i], NULL, thClient, &f) != 0)
			FatalErrorSystem("Erro na thread client");

	for(i=0; i<nClients; i++)
		if(pthread_join(th[i], NULL)!=0)
			FatalErrorSystem("Erro a terminar thread client");

	return 0;
}
