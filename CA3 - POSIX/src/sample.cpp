/*
 * Doughnut shop
 * A simple Producer-Consumer relationship with POSIX Threads
 * author: Kamran
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h> // For intptr_t

#define TRAY_CAPACITY 100

int tray = 0; // Tray [Critical]

pthread_mutex_t tray_mutex;
pthread_cond_t cond;

/*
 * Producer thread function
 */
void *bake(void *arg)
{
    int baker_id = (intptr_t)arg; // Safely convert void* to int
    int doughnuts = rand() % 25 + 1;

    pthread_mutex_lock(&tray_mutex);

    printf("\nBaker %d wants to bake %d doughnuts : Tray contains %d", baker_id, doughnuts, tray);

    while (doughnuts + tray > TRAY_CAPACITY)
    {
        pthread_cond_wait(&cond, &tray_mutex);
    }

    tray += doughnuts;

    printf("\nBaker %d baked %d doughnuts : Tray contains %d", baker_id, doughnuts, tray);

    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&tray_mutex);

    pthread_exit(NULL);
}

/*
 * Consumer thread function
 */
void *consume(void *arg)
{
    int customer_id = (intptr_t)arg; // Safely convert void* to int
    int doughnuts = rand() % 25 + 1;

    pthread_mutex_lock(&tray_mutex);

    printf("\nCustomer %d wants to consume %d doughnuts : Tray contains %d", customer_id, doughnuts, tray);

    while (tray - doughnuts < 0)
    {
        pthread_cond_wait(&cond, &tray_mutex);
    }

    tray -= doughnuts;

    printf("\nCustomer %d consumed %d doughnuts : Tray contains %d", customer_id, doughnuts, tray);

    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&tray_mutex);

    pthread_exit(NULL);
}

#define BAKERS 5
#define CUSTOMERS 5

int main(int argc, char **argv)
{
    int i;
    int result;
    void *t_return;
    pthread_t baker[BAKERS];
    pthread_t customer[CUSTOMERS];

    // Initialize mutex and condition variable
    result = pthread_mutex_init(&tray_mutex, NULL);
    if (result != 0)
    {
        perror("\nMutex initialization failed");
        exit(EXIT_FAILURE);
    }

    result = pthread_cond_init(&cond, NULL);
    if (result != 0)
    {
        perror("\nCondition variable initialization failed");
        exit(EXIT_FAILURE);
    }

    // Set seed for pseudo-random numbers
    srand(getpid());

    // Creating Baker and Customer threads
    for (i = 0; i < BAKERS; i++)
    {
        result = pthread_create(&baker[i], NULL, bake, (void *)(intptr_t)i);
        if (result != 0)
        {
            perror("\nThread creation failed");
            exit(EXIT_FAILURE);
        }
        printf("\nBaker %d ready to bake", i);
    }

    for (i = 0; i < CUSTOMERS; i++)
    {
        result = pthread_create(&customer[i], NULL, consume, (void *)(intptr_t)i);
        if (result != 0)
        {
            perror("\nThread creation failed");
            exit(EXIT_FAILURE);
        }
        printf("\nCustomer %d ready to consume", i);
    }

    // Wait for threads to finish execution
    for (i = 0; i < BAKERS; i++)
    {
        result = pthread_join(baker[i], &t_return);
        if (result != 0)
        {
            perror("\nThread join failed");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < CUSTOMERS; i++)
    {
        result = pthread_join(customer[i], &t_return);
        if (result != 0)
        {
            perror("\nThread join failed");
            exit(EXIT_FAILURE);
        }
    }

    // Clean-up mutex & condition variable
    pthread_mutex_destroy(&tray_mutex);
    pthread_cond_destroy(&cond);

    printf("\n");
    return 0;
}
