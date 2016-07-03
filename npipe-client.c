/*just a sample client to show how named pipes can be 
used to implement a client-server communication*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>

#define MAXBUFL 255

char *prog_name;

int main(int argc, char *argv[]){
	int fd;
	char recvline[MAXBUFL];
			/*blocks until a process opens the write terminal*/

	prog_name = argv[0];
	fd = open("npipe", O_RDONLY);
	if(fd < 0){
		return -1;
	}
	
	bzero(recvline, MAXBUFL);
	while(read(fd, recvline, MAXBUFL) > 0){
		fprintf(stdout, "[%s] --- received line: %s\n", prog_name, recvline); fflush(stdout);
		if(!strcmp(recvline, "exit"))
			break;	
		bzero(recvline, MAXBUFL);
	}
	close(fd);

	return 0;
}
