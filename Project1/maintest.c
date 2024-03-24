#include "process.h"

#define DIM_BUFFER 10


int test_get_acess_to_out(){
    char *cmd[] = {"pdftotext", "-layout", "01-Trabalho.pdf", "-", NULL};

	process_t *process = process_init_command(cmd);
	if (process == NULL) { perror("process_init_command "); exit(-1); }
	process_get_acess_stdout(process);
	process_start_execution(process);

	int fdout = open("01-Trabalho.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fdout < 0) { perror("Open File"); exit(-2); }

	char    buf[DIM_BUFFER];
	size_t nBytes;
	int     retval = 0;

	while ( (nBytes = process_read_out(process, buf, sizeof(buf))) > 0 ) {
		if (write(fdout, buf, nBytes) < 0) { perror("write"); retval = -3; break; }
	}
	if (nBytes < 0) { perror("read"); retval = -4; }

	int exitval;
	if ( process_wait_for_termination(process, &exitval) != 0 ){
		printf("\nProcess finished with exit value of %d\n\n", exitval);
	}

	close(fdout);
	process_destroy(process);

	return 0;
}
int test_get_acess_to_in(){
	char *cmd[] = {"wc", NULL};
	process_t *process = process_init_command(cmd);

	if (process == NULL) {
		perror("process_init_command ");
		exit(-1);
		}

	process_get_acess_stdin(process);
	process_start_execution(process);

	int fdin = open("01-Trabalho.txt",O_RDONLY);

	if (fdin < 0) {
		perror("Open File");
		exit(-2);
	}

	char buf[DIM_BUFFER];
	size_t nBytes;
	int retval = 0;

	while((nBytes=read(fdin,buf,sizeof(buf)))>0){
		if (process_write_in(process,buf,nBytes)<0) {
			perror("write");
			retval = -3;
			break;
		}
	}

	if (nBytes < 0) {
		perror("read");
		retval = -4;
	}

	int exitval;
	if ( process_wait_for_termination(process, &exitval) != 0 )
		printf("\nProcess finished with exit value of %d\n\n", exitval);

	 close(fdin);
	 process_destroy(process);

	return retval;
}

int main(){
	test_get_acess_to_out();
	test_get_acess_to_in();
return 0;
}
