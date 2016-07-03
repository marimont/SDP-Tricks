/*this is a very simple and basic program which shows how a pipeline actually works:
	- two processes are created
	- the first process stdout is redirected to a write pipe terminal
	- the second process stdin is redirected to a read pipe terminal
	- the two processes read/write from their stdin/stdout, without knowing that their filedes have been redirected
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void child_proc(int filedes[2]){
	char recvline[255];
					/*save stdin filedes*/
	int save_stdin = dup(0);
					/*close write pipe terminal*/
	close(filedes[1]);
					/*redirect stdin to read pipe terminal so that anything is written on the pipe
					can be read directly from stdin*/
	close(0);
	dup(filedes[0]);
					/*when "exit" is receive, restore the stdin and return*/
	bzero(recvline, sizeof(recvline));
	while(read(STDIN_FILENO, recvline, 255) > 0 && strcmp(recvline, "exit")){
			fprintf(stdout, "[child] --- Received string: %s\n", recvline);
			bzero(recvline, sizeof(recvline));
		}
					/*restore stdin*/
	close(0);
	dup(save_stdin);
}

void parent_proc(int filedes[2]){
	char sendline[255];
					/*save stdout*/
	int save_stdout = dup(1);
					/*close read pipe terminal*/
	close(filedes[0]);
	
					/*redirect stdout to write pipe terminal so that anything will be written on stdout, will
					be actually written on the pipe terminal*/
	close(1);
	dup(filedes[1]);
					/*when "exit" is read, restore the stdout and return*/
	while(1){
		fgets(sendline, 255, stdin);
		sscanf(sendline, "%s", sendline);
		if(!strcmp(sendline, "exit"))
			break;
		write(STDOUT_FILENO, sendline, strlen(sendline));
	}
					/*restore stdout*/
	close(1);
	dup(save_stdout);
}

int main(){
	int filedes[2];


	if( pipe(filedes) < 0)
		return -1;
	if(fork() == 0){
		child_proc(filedes);
		exit(0);
	}

	parent_proc(filedes);
	return 0;
}

