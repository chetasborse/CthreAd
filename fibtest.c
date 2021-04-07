#include "cthread.h"
#include <stdio.h>
#include <stdlib.h>

// evaluate a Fibonacci number:
//	fib(0) = 0
//	fib(1) = 1
//	fib(n) = fib(n-1) + fib(n-2)  [n>1]
// this function is messy because we have to pass everything as a
// generic parameter (void*).
// also, the function parameter is a value/result -- therefore it is a
// pointer to an integer.
//
void fib(void *in)
{
    int *n = (int *)in;
    //printf("\n In fib n=%d", *n);	 	/* cast input parameter to an int * */
    printf("WW: %d ", *n);
    if (*n == 0)
        /* pass */; /* return 0; it already is zero */

    else if (*n == 1)
        /* pass */; /* return 1; it already is one */

    else
    {
        int n1 = *n - 1; /* child 1 param */
        int n2 = *n - 2; /* child 2 param */

        // create children; parameter points to int that is initialized.
        // this is the location they will write to as well.
        cthread *c1 = (cthread *)cthread_create(fib, (void *)&n1);
        cthread *c2 = (cthread *)cthread_create(fib, (void *)&n2);
        // after creating children, wait for them to finish
        cthread_join((void *)c1);
        cthread_join((void *)c2);
        //  write to addr n_ptr points; return results in addr pointed to
        //  by input parameter
        *n = n1 + n2;
    }

    // MyThreadExit(); // always call this at end
}

int main(int argc, char *argv[])
{
    int n;

    if (argc != 2)
    {
        printf("usage: %s <n>\n", argv[0]);
        exit(-1);
    }
    n = atoi(argv[1]);
    if (n < 0 || n > 100)
    {
        printf("invalid value for n (%d)\n", n);
        exit(-1);
    }

    printf("fib(%d) = ", n);
    cthread_init(fib, (void *)&n);
    printf("%d\n", n);
}
