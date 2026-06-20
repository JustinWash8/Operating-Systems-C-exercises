#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t shutdown_flag = 0;
volatile sig_atomic_t stats_flag = 0;

void handle_sigint(int sig) {
        shutdown_flag = 1;
}

void handle_sigusr1(int sig) {
        stats_flag = 1;
}


int main(int argc, char *argv[] )  {

	FILE *file;
	char *filename = NULL;
	int buffer_size = 4096;
	char buffer[4096];
	int opt;

	   struct sigaction sa;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        sa.sa_handler = handle_sigint;
        sigaction(SIGINT, &sa, NULL);

        sa.sa_handler = handle_sigusr1;
        sigaction(SIGUSR1, &sa, NULL);

	while ((opt = getopt(argc, argv, "f:b:")) != -1)  {
		switch (opt) {
			case 'f':
				filename = optarg;
				break;
			case 'b':
				buffer_size = atoi(optarg);
				break;
			default:
				fprintf(stderr, "Usage: %s [-f file] [-b size]\n", argv[0]);
				exit(1);
		}
	}

	if (filename != NULL)  {
		file = fopen(filename, "r");
		if (file == NULL)  {
			printf("Error: could not open log file\n");
			return 1;
		}

		int chunks = 0;
		while (!shutdown_flag && fgets(buffer, sizeof(buffer), file) != NULL)  {
			printf("%s", buffer);
			chunks++;

			if(stats_flag) {
				fprintf(stderr, "Producer chunks writtern: %d\n", chunks);
				stats_flag = 0;
			}
		}
		fclose(file);
	}
}

