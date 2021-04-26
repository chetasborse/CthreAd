//
// Created by Wayne on 24/1/13.
//

#include <stdio.h>
#include "../cthread.h"

static long cnt = 0;

void *thread1(void *arg)
{
    int i;
    for (i = 0; i < 10; ++i)
    {
        cnt += 1;
        printf("%ld\n", cnt);
        cthread_yield();
    }
    cthread_exit(NULL);
}

void *thread2(void *arg)
{
    int i;

    for (i = 0; i < 10; ++i)
    {
        cnt *= 2;
        printf("%ld\n", cnt);
        cthread_yield();
    }

    cthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int a, b;
    cthread_init(1);

    a = cthread_create(thread1, NULL);
    b = cthread_create(thread2, NULL);

    cthread_join(a);
    cthread_join(b);

    printf("%ld\n", cnt);

    return 0;
}