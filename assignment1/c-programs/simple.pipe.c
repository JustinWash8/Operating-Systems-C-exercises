#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()  {
	int pipefd[2];
	pid_t pid;
	char buffer[100];
	char *message = "Hello from parent!";
	pipe(pipefd);

	if (pipe(pipefd) == -1){
		return 1;
	}

	pid = fork();

	if (pid == 0)  {
		close(pipefd[1]);
		read(pipefd[0], buffer, 100);
		printf("Read from pipe: %s\n", buffer );
		close(pipefd[0]);
	} else {
		close(pipefd[0]);
		write(pipefd[1], message, strlen(message));
		close(pipefd[1]);
		wait(NULL);
		}

	return 0;
}
