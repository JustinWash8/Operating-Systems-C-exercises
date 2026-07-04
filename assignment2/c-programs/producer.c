#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "buffer.h"
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>


int main(int argc, char *argv[] )  {

	if (argc != 3) {
                fprintf(stderr, "Usage: %s <producer_id> <num_items>\n", argv[0]);
                return 1;
        }

	int producer_id = atoi(argv[1]);
        int num_items = atoi(argv[2]);

	sem_t *mutex = sem_open("/mutex", O_CREAT, 0644, 1);
	sem_t *empty = sem_open("/empty", O_CREAT, 0644, BUFFER_SIZE);
	sem_t *full = sem_open("/full", O_CREAT, 0644, 0);

	if(mutex == SEM_FAILED) {
		perror("sem_open mutex");
		return 1;
	}

	 if(empty == SEM_FAILED) { 
                perror("sem_open empty");
                return 1;
        }

	 if(full == SEM_FAILED) { 
                perror("sem_open full");
                return 1;
        }

	int shm_id = shmget(SHM_KEY, sizeof(shared_buffer_t), IPC_CREAT |0666);
	shared_buffer_t *shared_buffer = (shared_buffer_t *) shmat(shm_id, NULL, 0);

	if (shared_buffer == (void *)-1) {
		perror("shmat");
		return 1;
	}

	for (int i = 0; i < num_items; i++)  {
		int value = producer_id * 1000 + i;

		sem_wait(empty);
		sem_wait(mutex);

		shared_buffer->buffer[shared_buffer->head].value = value;
		shared_buffer->buffer[shared_buffer->head].producer_id = producer_id;

		shared_buffer->head = (shared_buffer->head + 1) % BUFFER_SIZE;
		shared_buffer->count++;

		sem_post(mutex);
		sem_post(full);

		printf("producer %d produced %d\n", producer_id, value);

	}

	shmdt(shared_buffer);
	sem_close(mutex);
	sem_close(empty);
	sem_close(full);

}

