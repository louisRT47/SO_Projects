#ifndef _funcoessocket_h
#define _funcoessocket_h

#include "process.h"


void socketReceive_server(process_t *p, int sockfd, char *buf, int size);
void socketSend_server(process_t *p, int sockfd, char *buf);
void socketSend_client(int fd, char *buf, int dim, int sockfd);
void socketReceive_client(int fd, char *buf, int dim, int sockfd);


#endif
