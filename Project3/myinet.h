#ifndef _myinet_h
#define _myinet_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/* Endereços IP das maquinas de teste do ISEL/DEEC */
#define LOCALHOST	    "127.0.0.1"
#define DEFAULT_HOST	LOCALHOST


/* Porto utilizado pelo servidor */
#define DEFAULT_PORT		8000

#define DIM_BUFFER		512


#endif
