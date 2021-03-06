
// Simple producer-consumer program that tests the threads package.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "../cthread.h"

#define NUM_THREADS 8

int queue_size = 0;
int iterations = 10;

cthread_spinlock queue_lock;

void *producer(void *data)
{
    while (iterations)
    {
        cthread_spinlock_lock(&queue_lock);
        queue_size++;
        printf("    Produced: virtual queue has size %d\n", queue_size);
        fflush(stdout);
        iterations--;
        cthread_spinlock_unlock(&queue_lock);
        cthread_yield();
    }

    cthread_exit(NULL);
    return NULL;
}

void *consumer(void *data)
{
    while (iterations)
    {
        cthread_spinlock_lock(&queue_lock);

        if (queue_size == 0)
        {
            printf("    DID NOT CONSUME: virtual queue has size %d\n", queue_size);
            fflush(stdout);
        }
        else
        {
            queue_size--;
            printf("    Consumed: virtual queue has size %d\n", queue_size);
            fflush(stdout);
        }

        iterations--;
        cthread_spinlock_unlock(&queue_lock);
        cthread_yield();
    }

    cthread_exit(NULL);
    return NULL;
}

int main(void)
{
    int producer1;
    int consumer1;
    int consumer2;

    // Initialize the threading package
    cthread_init(1);

    // Initialize the mutex
    cthread_spinlock_init(&queue_lock);

    // Initialize threads
    producer1 = cthread_create(producer, NULL);
    consumer1 = cthread_create(consumer, NULL);
    consumer2 = cthread_create(consumer, NULL);

    // Join threads
    cthread_join(producer1);

    return 0;
}
