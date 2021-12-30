#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct{
	int *io_child;
    int *io_parent;
	char *command;
}args;	

int main(int argc, char const *argv[])
{ 
    char* argve[] = {};
	char* envp[] = {};	
	args *arg = (args *)malloc(sizeof(args));
	arg->io_child = (int *)malloc(2*sizeof(int));	
	arg->io_parent = (int *)malloc(2*sizeof(int));	
	char *command = "/bin/python3\0";
	arg->command = (char *)malloc(strlen(command)*sizeof(char));
	for (int i = 0; i < strlen(command); ++i)
	{
		arg->command[i] = command[i];
	}
	int nbytes = 0;
    int rbytes = 0;
	char *buffer = (char *)malloc(sizeof(char));
    int fork_pid = fork();
    if (fork_pid < 0)
    {
        perror("Fork error.");
        exit(1);
    }
    else if (fork_pid == 0)
    {
        pipe(arg->io_child);
        dup2(arg->io_child[1], 0);
        dup2(arg->io_child[0], 1);
        arg->io_child[1] = fcntl(arg->io_child[1], F_SETFD, 0);
        arg->io_child[0] = fcntl(arg->io_child[0], F_SETFD, 0);
        execve(arg->command, argve, envp);
    }else
    {   
        char *com = "2**4\n";
        for(int i = 0; i < strlen(com); ++i)
        {
            write(arg->io_child[1], &com[i], sizeof(com[i]));
        }
        while ((rbytes = read(arg->io_child[0], &buffer[nbytes], sizeof(buffer[nbytes]))) > 0){
            nbytes++;
            buffer = (char *)realloc(buffer, nbytes+1); 
        }
        printf("nbytes = %d\n", nbytes);
	    buffer[nbytes+1] = '\0';  
	    printf("%s", buffer);
    }
    close(arg->io_child[1]);
    close(arg->io_child[0]);
    close(arg->io_parent[1]);
    close(arg->io_parent[0]);
    free(buffer);
    free(arg->command);
    free(arg->io_parent);
    free(arg->io_child);
    free(arg);
	return 0;
}
