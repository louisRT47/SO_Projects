#include "sBuffer.h"

void sharedBuffer_init (SharedBuffer *sb, int capacity)
{
	sb->buffer = (void **)malloc(capacity *sizeof(void *));
	sb->iGet = 0;
	sb->iPut = 0;
	sb->nelems = 0;
	sb->maxCapacity = capacity;
	pthread_cond_init(&sb->cEsperaEspacoLivre, NULL);
	pthread_cond_init(&sb->cEsperaEspacoOcupado, NULL);
	pthread_mutex_init(&sb->mutex, NULL);
}
void sharedBuffer_destroy (SharedBuffer *sb)
{
free(sb->buffer);
pthread_cond_destroy(&sb->cEsperaEspacoLivre);
pthread_cond_destroy(&sb->cEsperaEspacoOcupado);
pthread_mutex_destroy(&sb->mutex);
}

void sharedBuffer_Put (SharedBuffer *sb, void *data)
{
	pthread_mutex_lock(&sb->mutex);
	while (sb->nelems == sb->maxCapacity) {
		pthread_cond_wait(&sb->cEsperaEspacoLivre,&sb->mutex);
	}
	sb->buffer[sb->iPut] = data;
	sb->iPut =	(sb->iPut+1)% sb->maxCapacity;
	++sb->nelems;
	pthread_cond_signal(&sb->cEsperaEspacoOcupado);
	pthread_mutex_unlock(&sb->mutex);
}

void * sharedBuffer_Get (SharedBuffer *sb)
{
	void *ret;
	pthread_mutex_lock(&sb->mutex);
		while (sb->nelems == 0) {
			pthread_cond_wait(&sb->cEsperaEspacoOcupado,&sb->mutex);
		}
	ret=sb->buffer[sb->iGet];
	sb->iGet =	(sb->iGet+1)% sb->maxCapacity;
	--sb->nelems;
	pthread_cond_signal(&sb->cEsperaEspacoLivre);
	pthread_mutex_unlock(&sb->mutex);
return ret;
}
