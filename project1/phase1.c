/*
 *  Name: Justin Washington
 *  Assignment: 3
 *  Phase 1 - Basic Threads
 *  Filename: phase1.c
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

#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *t1_deposit(void *arg);
void *t2_deposit(void *arg);
void *t3_deposit(void *arg);
void *t4_deposit(void *arg);

int seed_savings_account();
void *close_the_bank();

int INITIAL_AMOUNT = 100;
char * ACCT_NAME = "savings.txt";

int main(void) {
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;

    // Make an initial deposit.
    seed_savings_account();

    // Create 4 threads to make depostis.
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

    // Close the bank and check the closing balance.
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

    // Open the file to read the current amount.
    file_ptr = fopen(ACCT_NAME, "r+");
    if (file_ptr == NULL) {
        printf("Thread 1 couldn't access the bank account.\n");
        return (void  *) (intptr_t) 1;
    }

    if (fscanf(file_ptr, "%d", &number) == 1) {
        printf("Thread 1, with thread ID: %lu, read %d in the account and added %d.\n", 
                (unsigned long) id, number, deposit_amount);
        rewind(file_ptr);
        fprintf(file_ptr, "%d\n", number + deposit_amount);
        fclose(file_ptr);   // close the file handle to make it available.
    } else {
        printf("Error: Thread 1 could not read a valid integer from the file.\n");
        return (void  *) (intptr_t) 1;
    }

    return 0;
}

/*
 * thread2 deposit
 */
void *t2_deposit(void * arg) {
    FILE *file_ptr;
    int deposit_amount = 2;
    int number;
    pthread_t id = pthread_self();

    // Open the file to read the current amount.
    file_ptr = fopen(ACCT_NAME, "r+");
    if (file_ptr == NULL) {
        printf("Thread 2 couldn't access the bank account.\n");
        return (void  *) (intptr_t) 1;
    }

    if (fscanf(file_ptr, "%d", &number) == 1) {
        printf("Thread 2, with thread ID: %lu, read %d in the account and added %d.\n", 
                (unsigned long) id, number, deposit_amount);
        rewind(file_ptr);
        fprintf(file_ptr, "%d\n", number + deposit_amount);
        fclose(file_ptr);   // close the file handle to make it available.
    } else {
        printf("Error: Thread 2 could not read a valid integer from the file.\n");
        return (void  *) (intptr_t) 1;
    }

    return 0;
}

/*
 * thread3 deposit
 */
void *t3_deposit(void * arg) {
    FILE *file_ptr;
    int deposit_amount = 3;
    int number;
    pthread_t id = pthread_self();

    // Open the file to read the current amount.
    file_ptr = fopen(ACCT_NAME, "r+");
    if (file_ptr == NULL) {
        printf("Thread 3 couldn't access the bank account.\n");
        return (void  *) (intptr_t) 1;
    }

    if (fscanf(file_ptr, "%d", &number) == 1) {
        printf("Thread 3, with thread ID: %lu, read %d in the account and added %d.\n", 
                (unsigned long) id, number, deposit_amount);
        rewind(file_ptr);
        fprintf(file_ptr, "%d\n", number + deposit_amount);
        fclose(file_ptr);   // close the file handle to make it available.
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

    // Open the file to read the current amount.
    file_ptr = fopen(ACCT_NAME, "r+");
    if (file_ptr == NULL) {
        printf("Thread 4 couldn't access the bank account.\n");
        return (void  *) (intptr_t) 1;
    }

    if (fscanf(file_ptr, "%d", &number) == 1) {
        printf("Thread 4, with thread ID: %lu, read %d in the account and added %d.\n", 
                (unsigned long) id, number, deposit_amount);
        rewind(file_ptr);
        fprintf(file_ptr, "%d\n", number + deposit_amount);
        fclose(file_ptr);   // close the file handle to make it available.
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
    fclose(file_ptr);   // close the file handle to make it available.

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
        printf("The closing balance is %d, in the account \"%s\".\n", closing_balance, ACCT_NAME);
        fclose(file_ptr);
    } else {
        printf("Error: Unable to read closing balance.\n");
        fclose(file_ptr);   // close the file handle and end the program.
        return (void *) (intptr_t) 1;
    }

    return 0;
}
