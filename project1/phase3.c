/*
 *  Name: Justin Washington
 *  Assignment: 3
 *  Phase 3 - Deadlock Creation
 *  Filename: phase3.c
 *  Description: Create a program with no synchronization and 
 *               the potential for deadlocks.
 *
 *  Scenario: The program has two arrays of 100,000 integers each.
 *            The two arrays are named red and blue, respectively.
 *            The goal is to swap the numbers from one array to the
 *            other using a bounded buffer of size 100 in the middle.
 *            The size of the arrays will not grow, and at the end 
 *            of the transfer, they'll each be the original size
 *            of 100,000 integers.
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define BUFFER_SIZE 100
#define NUM_ITEMS 100000

char red_rpt_start[80];
char red_rpt_finish[80];
char green_rpt_start[80];
char green_rpt_finish[80];

int buffer[BUFFER_SIZE];
int count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* producer(void* arg) {
    pthread_t id = pthread_self();
    printf("Starting producer thread with id: %lu\n", id);

    int* source = (int*)arg;

    for (int i = 0; i < NUM_ITEMS; i++) {
        // Busy wait - inefficient and not true synchronization
        while (count == BUFFER_SIZE) { } 
        pthread_mutex_lock(&lock);
        buffer[count++] = source[i];
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* consumer(void* arg) {
    pthread_t id = pthread_self();
    printf("Starting consumer thread with id: %lu\n", id);

    int* dest = (int*)arg;

    for (int i = 0; i < NUM_ITEMS; i++) {
        // Busy wait - inefficient
        while (count == 0) { } 
        pthread_mutex_lock(&lock);
        dest[i] = buffer[--count];
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void initialize_data(int* red, int* green) {
    printf("Initializing Red Array from 1 – 100,000\n");
    printf("Initializing Green Array from 100,001 – 200,000\n");
    for (int i = 0; i < NUM_ITEMS; i++) {
        // Red array gets 1 to 1,000,000
        red[i] = i + 1;
        // Green array gets 1,000,001 to 2,000,000
        green[i] = i + 100001;
    }
    snprintf(red_rpt_start, sizeof(red_rpt_start),"  red_array[0]: %d          red_array[99999]: %'d\n", 
            red[0], red[NUM_ITEMS - 1]);
    snprintf(green_rpt_start, sizeof(green_rpt_start),"green_array[0]: %d   green_array[199999]: %'d\n", 
            green[0], green[NUM_ITEMS - 1]);
}

int main() {
    setlocale(LC_NUMERIC,"");
    int* red_array = malloc(NUM_ITEMS * sizeof(int));
    int* green_array = malloc(NUM_ITEMS * sizeof(int));
    // Initialize data...
    initialize_data(red_array, green_array);

    pthread_t p1, c1;
    pthread_create(&p1, NULL, producer, green_array);
    pthread_create(&c1, NULL, consumer, red_array);
    pthread_join(p1, NULL);
    pthread_join(c1, NULL);
    printf("Transfer complete (Phase 3).\n");

    snprintf(red_rpt_finish, sizeof(red_rpt_finish), "  red_array[0]: %d     red_array[99999]: %'d\n", 
            red_array[0], red_array[NUM_ITEMS - 1]);
    snprintf(green_rpt_finish, sizeof(green_rpt_finish),"green_array[0]: %d   green_array[199999]: %'d\n", 
            green_array[0], green_array[NUM_ITEMS - 1]);

    free(red_array); free(green_array);
    printf("\nInitialized Arrays:\n%s%s", red_rpt_start, green_rpt_start);
    printf("\nSwapped Arrays with Deadlocks:\n%s%s", red_rpt_finish, green_rpt_finish);
    return 0;
}
