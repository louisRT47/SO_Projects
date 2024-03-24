#include "process.h"


process_t *process_init_command(char *cmd[]){
	process_t *save =  malloc(sizeof(process_t));
	save->args=cmd;
	save->fileIN=0;
	save->fileOUT=0;
	save->acessIN=0;
	save->acessOUT=0;
	return save;
}

int process_set_stdout_to_file(process_t *process, int file){
	process->fd = file;
	process->fileOUT=1;
	return 0;
}
int process_set_stdin_to_file(process_t *process, int file){
	process->fd =file;
	process->fileIN=1;
	return 0;
}

int process_get_acess_stdout(process_t *process){
	if(pipe(process->pipeOUT) < 0){
		perror("Erro na criação do pipeOUT");
		exit(-1);
	}
	process->acessOUT=1;
	return 0;
}

int process_get_acess_stdin(process_t *process){
	if(pipe(process->pipeIN) < 0){
		perror("Erro na criação do pipeIN");
		exit(-1);
	}
	process->acessIN=1;
	return 0;
}

int process_start_execution(process_t *process){

	process->process_id = fork();

	if(process->process_id < 0){
		perror("Erro na criação do processo");
		exit(-1);
	}

	if(process->process_id == 0){
		printf("Sou o processo filho com pid = %d\n", getpid());

		if (process->fileOUT)
			dup2(process->fd, STDOUT_FILENO);
		if (process->fileIN)
			dup2(process->fd, STDIN_FILENO);
		if (process->acessOUT){
			dup2(process->pipeOUT[1], STDOUT_FILENO);
			close(process->pipeOUT[1]);
			close(process->pipeOUT[0]);
		}
		if (process->acessIN){
			dup2(process->pipeIN[0], STDIN_FILENO);
			close(process->pipeIN[0]);
			close(process->pipeIN[1]);
		}
		execvp(process->args[0], process->args);
		perror("Erro no exec");
		return -1;
	}
	else{
		if(process->acessIN)
			close(process->pipeIN[0]);
		if(process->acessOUT)
			close(process->pipeOUT[1]);
	}
	return 0;
}

int process_wait_for_termination(process_t * process, int *exitValue){
		if(process->acessIN)
			close(process->pipeIN[1]);
		if(process->acessOUT)
			close(process->pipeOUT[0]);

		int status;
		waitpid(process->process_id,&status,0);
		if(WIFEXITED(status)){
			*exitValue=WEXITSTATUS(status);
			printf("\nO processo filho terminou correctamente com um pid de %d", process->process_id);
			return 1;
		}
		return 0;
}

void process_destroy(process_t *process){
	free(process);
}

int process_read_out(process_t *process, void * buf, size_t size){
		int nbytes = read(process->pipeOUT[0], buf, size);
		if (nbytes < 0) perror ("Erro no read");
		return nbytes;
}
int process_write_in(process_t *process,void * buf,size_t size){
		int nbytes = write(process->pipeIN[1], buf, size);
		if(nbytes < 0)	perror ("Erro no write");
		return nbytes;
}

