#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cthread.h"

int counter = 0;

void *func(void *args)
{
    for (int i = 0; i < 20; i++)
    {
        counter++;
    }

    cthread_exit(NULL);
}

void *func2(void *args)
{
    for (int i = 0; i < 20; i++)
    {
        counter++;
    }

    cthread_exit(NULL);
}

int main()
{
    int t1, t2, t3;
    cthread_init(1);

    t1 = cthread_create(func, NULL);
    t2 = cthread_create(func2, NULL);

    cthread_join(t1);
    cthread_join(t2);

    printf("Counter=%d\n", counter);
    return 0;
}