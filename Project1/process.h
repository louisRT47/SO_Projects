#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

typedef struct{
	pid_t process_id;
	char **args;
	int fd;
	int pipeOUT[2], pipeIN[2];
	//flags
	int fileIN, fileOUT;
	int acessIN, acessOUT;
}process_t;

process_t *process_init_command(char *cmd[]);

int process_set_stdout_to_file(process_t *process, int file);

int process_get_acess_stdout(process_t *process);

int process_start_execution(process_t *process);

int process_wait_for_termination(process_t * process, int *exitValue);

void process_destroy(process_t *process);

int process_read_out(process_t *process, void * buf, size_t size);
//extra
int process_set_stdin_to_file(process_t *process, int file);

int process_get_acess_stdin(process_t *process);

int process_write_in(process_t *process,void * buf,size_t size);


#endif /* PROCESS_H_ */
