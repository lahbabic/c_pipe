#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
	int* io;
	char* command;
}args;	

int main(int argc, char const *argv[])
{
	int fork_pid = fork();
	char* argve[] = { NULL };
	char* envp[] = { NULL };	
	args *arg = (args *)malloc(sizeof(args));
	arg->io = (int *)malloc(2*sizeof(int));	
	char *command = "/bin/python3";
	arg->command = (char *)malloc(strlen(command)*sizeof(char));
	for (int i = 0; i < strlen(command); ++i)
	{
		arg->command[i] = command[i];
	}
	int nbytes = 0;
	char buffer[1025];

    if (fork_pid == 0){
    	execve(arg->command, argve, envp);	
    	pipe(arg->io);
    }else{
    	sleep(5);
		write(arg->io[1], "2**4", 6);
		do
		{
			nbytes = read(arg->io[0], buffer, sizeof(buffer));
		}while( nbytes == -1 );
		buffer[nbytes] = '\0';  
	    printf("%s\n", buffer);
    }

	return 0;
}