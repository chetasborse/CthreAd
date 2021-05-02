/**
 * Example code for using spinlocks. This program creates 5 threads
 * each of which tries to increment two variables - one shared and one
 * individual. To access the shared variable spinlocks are used to guarantee
 * mutual exclusion and race conditions. Absence of race conditions are checked
 * by checking if the sum of inidividual values is same as that of the shared
 * variable.
 */
// Test inspired by code of Mayank Jain
#include "../cthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define print(str) write(1, str, strlen(str))

long long c = 0, c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, running = 1;

cthread_spinlock lock;

void *thread1(void *arg)
{
    while (running == 1)
    {
        cthread_spinlock_lock(&lock);
        c++;
        cthread_spinlock_unlock(&lock);
        c1++;
        cthread_yield();
    }
    cthread_exit(NULL);
}

void *thread2(void *arg)
{
    while (running == 1)
    {

        cthread_spinlock_lock(&lock);
        c++;
        cthread_spinlock_unlock(&lock);
        c2++;
        cthread_yield();
    }
    cthread_exit(NULL);
}

void *thread3(void *arg)
{
    while (running == 1)
    {

        cthread_spinlock_lock(&lock);
        c++;
        cthread_spinlock_unlock(&lock);
        c3++;
        cthread_yield();
    }
    cthread_exit(NULL);
}

void *thread4(void *arg)
{
    while (running == 1)
    {

        cthread_spinlock_lock(&lock);
        c++;
        cthread_spinlock_unlock(&lock);
        c4++;
        cthread_yield();
    }
    cthread_exit(NULL);
}

void *thread5(void *arg)
{
    while (running == 1)
    {

        cthread_spinlock_lock(&lock);
        c++;
        cthread_spinlock_unlock(&lock);
        c5++;
        cthread_yield();
    }
    cthread_exit(NULL);
}

int main(int argc, char **argv)
{
    int tids[6];

    fprintf(stdout, "    Thread Spinlocks\n");

    cthread_init(1);
    cthread_spinlock_init(&lock);

    tids[1] = cthread_create(thread1, NULL);
    tids[2] = cthread_create(thread2, NULL);
    tids[3] = cthread_create(thread3, NULL);
    tids[4] = cthread_create(thread4, NULL);
    tids[5] = cthread_create(thread5, NULL);
    fprintf(stdout, "    Created 5 threads\n");
    fprintf(stdout, "    Letting threads run\n");

    for (long long int i = 0; i < 1000000000; i++)
        ;

    running = 0;

    cthread_join(1);
    cthread_join(2);
    cthread_join(3);
    cthread_join(4);
    cthread_join(5);
    fprintf(stdout, "    Joined on all 5 threads\n");

    fprintf(stdout, "    Thread 1 		= %lld\n", c1);
    fprintf(stdout, "    Thread 2 		= %lld\n", c2);
    fprintf(stdout, "    Thread 3 		= %lld\n", c3);
    fprintf(stdout, "    Thread 4 		= %lld\n", c4);
    fprintf(stdout, "    Thread 5 		= %lld\n", c5);
    fprintf(stdout, "    t1 + t2 + t3 + t4 + t5  = %lld\n", c1 + c2 + c3 + c4 + c5);
    fprintf(stdout, "    Shared Variable         = %lld\n", c);
    if (c1 + c2 + c3 + c4 + c5 == c)
    {
        fprintf(stdout, "    TEST PASSED\n");
    }
    else
    {
        fprintf(stdout, "    Test failed\n");
    }
    fflush(stdout);

    return 0;
}