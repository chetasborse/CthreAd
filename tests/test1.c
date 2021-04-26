
#include "cthread.h"
#include <stdio.h>
#include <stdlib.h>

void fib(void *in)
{
    int *n = (int *)in;

    if (*n == 0)
        /* pass */; /* return 0; it already is zero */

    else if (*n == 1)
        /* pass */; /* return 1; it already is one */

    else
    {
        int n1 = *n - 1;
        int n2 = *n - 2;

        cthread_create(fib, (void *)&n1);
        cthread_create(fib, (void *)&n2);

        cthread_join_all();

        *n = n1 + n2;
    }

    cthread_exit();
}

int main(int argc, char *argv[])
{
    int n;

    if (argc != 2)
    {
        printf("usage: ./prog <n>\n");
        exit(-1);
    }
    n = atoi(argv[1]);
    if (n < 0 || n > 10)
    {
        printf("invalid value for n (%d)\n", n);
        exit(-1);
    }

    printf("fib(%d) = ", n);
    cthread_init(fib, (void *)&n);
    printf("%d\n", n);
}
