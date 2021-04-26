#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cthread.h"

int counter = 0;

void *func(void *args)
{
    int i = 4;
    while (i > 0)
    {
        printf("Hello1!\n");
        sleep(1);
        cthread_yield();
        i--;
    }
    cthread_exit(NULL);
}

void *func2(void *args)
{
    int i = 4;
    while (i > 0)
    {
        printf("Hello2!\n");
        cthread_yield();
        sleep(1);
        --i;
    }
    cthread_exit(NULL);
}

int main()
{
    int t1, t2;
    cthread_init(1);

    t1 = cthread_create(func, NULL);
    t2 = cthread_create(func2, NULL);

    cthread_join(t1);
    cthread_join(t2);

    printf("Counter=%d\n", counter);
    return 0;
}