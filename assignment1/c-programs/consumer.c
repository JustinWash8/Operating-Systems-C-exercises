#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>

volatile sig_atomic_t shutdown_flag = 0;
volatile sig_atomic_t stats_flag = 0;

void handle_sigint(int sig) {
	shutdown_flag = 1;
}

void handle_sigusr1(int sig) {
	stats_flag = 1;
}

int main(int argc, char *argv[])  {

        int max_lines = -1;
	int verbose = 0;
        char buffer[4096];
	int lines = 0;
	int characters = 0;
        int opt;

	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sa.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa, NULL);

	sa.sa_handler = handle_sigusr1;
        sigaction(SIGUSR1, &sa, NULL);


        while ((opt = getopt(argc, argv, "n:v:")) != -1)  {
                switch (opt) {
                        case 'n':
                                max_lines = atoi(optarg);
                                break;
                        case 'v':
                                verbose = 1;
                                break;
                        default:
                                fprintf(stderr, "Usage: %s [-n file] [-v size]\n", argv[0]);
                                exit(1);
                }
        }

        while (!shutdown_flag && fgets(buffer, sizeof(buffer), stdin) != NULL)  {
		if(max_lines != -1 && lines >= max_lines) {
			break;
		}
		lines++;
		characters += strlen(buffer);
		if(verbose) {
			printf("%s", buffer);
		}

		if(stats_flag) {
			fprintf(stderr, "Current Lines: %d\n", lines);
        		fprintf(stderr, "Current Characters: %d\n", characters);
			stats_flag = 0;
		}
		fprintf(stderr, "Final Lines: %d\n", lines);
        	fprintf(stderr, "Final Characters: %d\n", characters);
        }
	fprintf(stderr, "Lines: %d\n", lines);
	fprintf(stderr, "characters: %d\n", characters);

	return 0;
}
