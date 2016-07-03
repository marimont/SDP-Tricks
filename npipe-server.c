/*just a sample server to show how named pipes can be 
used to implement a client-server communication*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>

#define MAXBUFL 255

char *prog_name;

int main(int argc, char *argv[]){

	int fd;
	char sendline[MAXBUFL];
 						/*17 corresponds to fifo already existing -> mkfifo will fail to creat it*/
	if( mkfifo("npipe", 0666) < 0 && errno != 17){
		fprintf(stderr, "[%s] --- cannot create fifo - %s\n", prog_name, strerror(errno));	
		return -1;
	}

	prog_name = argv[0];
			/*blocks until a process opens the read terminal*/
	fd = open("npipe", O_WRONLY);
	if(fd < 0){
		return -1;
	}
	fprintf(stdout, "[%s] --- connected to client\n", prog_name);
	fprintf(stdout, "> ");
	while(fscanf(stdin, "%s", sendline) != EOF){
		fprintf(stdout, "[%s] --- sending line: %s\n", prog_name, sendline);
		write(fd, sendline, strlen(sendline));
		bzero(sendline, MAXBUFL);
		fprintf(stdout, "> ");
	}
	fprintf(stdout, "[%s] --- sending exit line\n", prog_name);
	write(fd, "exit", 4);

	close(fd);

	return 0;
}
