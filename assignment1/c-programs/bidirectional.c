#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	int pipe1[2];
	int pipe2[2];
	pid_t pid;
	char parent_message[] = "Hello from parent!";
	char child_message[] = "Hello from child!";
	char buffer[100];

	if(pipe(pipe1) == -1) {
		printf("Error: pipe1 failed\n");
		return 1;
	}

	if(pipe(pipe2) == -1) {
		printf("Error: pipe2 failed\n");
		return 1;
	}
	pid = fork();
	if(pid == -1) {
		printf("Error: fork failed\n");
		return 1;
	}

	if(pid == 0) {
		close(pipe1[1]);
		close(pipe2[0]);
		read(pipe1[0], buffer, sizeof(buffer));
		printf("Child recieved: %s\n", buffer);

		write(pipe2[1], child_message, strlen(child_message) + 1);
		close(pipe1[0]);
		close(pipe2[1]);
	} else{
		close(pipe1[0]);
                close(pipe2[1]);
                write(pipe1[1], parent_message, strlen(parent_message) + 1);

                read(pipe2[0], buffer, sizeof(buffer));
		printf("Parent recieved: %s\n", buffer);
                close(pipe1[1]);
                close(pipe2[0]);

		wait(NULL);
	}
}


