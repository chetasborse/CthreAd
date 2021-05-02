
//
// Created by Wayne on 24/1/13.
//

#include <stdio.h>
#include "../cthread.h"

static long sum = 0;

void *thread(void *arg)
{
    sum += (long)arg;

    cthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int tids[MAX_THREADS];
    long i;

    cthread_init(1);

    for (i = 0; i < MAX_THREADS - 1; ++i)
    {
        tids[i] = cthread_create(thread, (void *)i);
    }

    for (i = 0; i < MAX_THREADS - 1; ++i)
    {
        cthread_join(tids[i]);
    }

    printf("%ld\n", sum);

    return 0;
}