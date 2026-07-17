/*
 *  Name: Justin Washington
 *  Assignment: 3
 *  Phase 2 - Mutex Protection 
 *  Filename: phase2.c
 *  Description: Basic threading with race conditions.
 *
 *  Note: When all 4 deposits are successful, the closing 
 *        balance will equal 110.
 *
 *        Seed deposit    100
 *        Thread1 deposit   1
 *        Thread2 deposit   2
 *        Thread3 deposti   3
 *        Thread4 deposit   4
 *        ___________________
 *                        110
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *t1_deposit(void *arg);
void *t2_deposit(void *arg);
void *t3_deposit(void *arg);
void *t4_deposit(void *arg);

int seed_savings_account();
void *close_the_bank();

char * ACCT_NAME = "savings.txt";
int INITIAL_AMOUNT = 100;

// Make new_amount a global variable so each thread has easy access to it.
// This makes it easy to add Mutex Protection without passing the variable
// around to different functions.
int new_amount = 0;
pthread_mutex_t new_amount_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;

    seed_savings_account();

    if (pthread_create(&thread1, NULL, t1_deposit, NULL) != 0) {
        perror("Failed to create thread 1");
    }

    if (pthread_create(&thread2, NULL, t2_deposit, NULL) != 0) {
        perror("Failed to create thread 2");
    }

    if (pthread_create(&thread3, NULL, t3_deposit, NULL) != 0) {
        perror("Failed to create thread 3");
    }

    if (pthread_create(&thread4, NULL, t4_deposit, NULL) != 0) {
        perror("Failed to create thread 4");
    }

    // Synchronize these 4 threads to insure they each run to completion.
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    close_the_bank();
    return 0;
}

/*
 * thread1 deposit
 */
void *t1_deposit(void *arg) {
    FILE *file_ptr;
    int deposit_amount = 1;
    int number;
    pthread_t id = pthread_self();

    // Puts a lock the new_amount variable with a Mutex to control access to the variable.
    pthread_mutex_lock(&new_amount_mutex);

    // Open the file to read the current amount.
    file_ptr = fopen(ACCT_NAME, "r+");
    if (file_ptr == NULL) {
        printf("Thread 1 couldn't access the bank account.\n");
        return (void  *) (intptr_t) 1;
    }

    if (fscanf(file_ptr, "%d", &number) == 1) {
        printf("Thread 1, with thread ID: %lu, read %d in the account and added 1.\n", 
                (unsigned long) id, number);
        new_amount = number + deposit_amount;
        rewind(file_ptr);
        fprintf(file_ptr, "%d\n", new_amount);
        fclose(file_ptr);
        // remove the Mutex
        pthread_mutex_unlock(&new_amount_mutex);
    } else {
        printf("Error: Thread 1 could not read a valid integer from the file.\n");
        return (void  *) (intptr_t) 1;
    }

    return 0;
}

/*
 * thread2 deposit
 */
void *t2_deposit(void *arg) {
    FILE *file_ptr;
    int deposit_amount = 2;
    int number;
    pthread_t id = pthread_self();

    // lock the new_amount variable with a Mutex.
    pthread_mutex_lock(&new_amount_mutex);

    // Open the file to read the current amount.
    file_ptr = fopen(ACCT_NAME, "r+");
    if (file_ptr == NULL) {
        printf("Thread 2 couldn't access the bank account.\n");
        return (void  *) (intptr_t) 1;
    }

    if (fscanf(file_ptr, "%d", &number) == 1) {
        printf("Thread 2, with thread ID: %lu, read %d in the account and added 1.\n", 
                (unsigned long) id, number);
        new_amount = number + deposit_amount;
        rewind(file_ptr);
        fprintf(file_ptr, "%d\n", new_amount);
        fclose(file_ptr);
        // remove the Mutex
        pthread_mutex_unlock(&new_amount_mutex);
    } else {
        printf("Error: Thread 2 could not read a valid integer from the file.\n");
        return (void  *) (intptr_t) 1;
    }

    return 0;
}

/*
 * thread3 deposit
 */
void *t3_deposit(void *arg) {
    FILE *file_ptr;
    int deposit_amount = 3;
    int number;
    pthread_t id = pthread_self();

    // lock the new_amount variable with a Mutex.
    pthread_mutex_lock(&new_amount_mutex);

    // Open the file to read the current amount.
    file_ptr = fopen(ACCT_NAME, "r+");
    if (file_ptr == NULL) {
        printf("Thread 3 couldn't access the bank account.\n");
        return (void  *) (intptr_t) 1;
    }

    if (fscanf(file_ptr, "%d", &number) == 1) {
        printf("Thread 3, with thread ID: %lu, read %d in the account and added 1.\n", 
                (unsigned long) id, number);
        new_amount = number + deposit_amount;
        rewind(file_ptr);
        fprintf(file_ptr, "%d\n", new_amount);
        fclose(file_ptr);
        // remove the Mutex
        pthread_mutex_unlock(&new_amount_mutex);
    } else {
        printf("Error: Thread 3 could not read a valid integer from the file.\n");
        return (void  *) (intptr_t) 1;
    }

    return 0;
}

/*
 * thread4 deposit
 */

void *t4_deposit(void *arg) {
    FILE *file_ptr;
    int deposit_amount = 4;
    int number;
    pthread_t id = pthread_self();

    // lock the new_amount variable with a Mutex.
    pthread_mutex_lock(&new_amount_mutex);

    // Open the file to read the current amount.
    file_ptr = fopen(ACCT_NAME, "r+");
    if (file_ptr == NULL) {
        printf("Thread 4 couldn't access the bank account and added 4.\n");
        return (void  *) (intptr_t) 1;
    }

    if (fscanf(file_ptr, "%d", &number) == 1) {
        printf("Thread 4, with thread ID: %lu, read %d in the account and added 1.\n", 
                (unsigned long) id, number);
        new_amount = number + deposit_amount;
        rewind(file_ptr);
        fprintf(file_ptr, "%d\n", new_amount);
        fclose(file_ptr);
        // remove the Mutex
        pthread_mutex_unlock(&new_amount_mutex);
    } else {
        printf("Error: Thread 4 could not read a valid integer from the file.\n");
        return (void  *) (intptr_t) 1;
    }

    return 0;
}

/*
 * seed the account with the first deposit.
 */
int seed_savings_account() {
    FILE *file_ptr;
    
    file_ptr = fopen(ACCT_NAME,"w");
    if (file_ptr == NULL) {
        printf("Could not create bank account\n");
        return 1;
    }

    fprintf(file_ptr, "%d\n", INITIAL_AMOUNT);
    fclose(file_ptr);

    return 0;
}

/*
 * Close the Bank
 */
void *close_the_bank() {
    FILE *file_ptr;
    int closing_balance = 0;

    file_ptr = fopen(ACCT_NAME, "r");
    if (file_ptr == NULL) {
        printf("Unable to open file to read closing balance.\n");
        return (void *) (intptr_t) 1;
    }

    if (fscanf(file_ptr, "%d", &closing_balance) == 1) {
        printf("The closing balance is %d\n", closing_balance);
        fclose(file_ptr);
    } else {
        printf("Error: Unable to read closing balance.\n");
        fclose(file_ptr);
        return (void *) (intptr_t) 1;
    }

    return 0;
}
