#include <stdio.h>
#include <unistd.h>
#include "cthread.h"

typedef struct nums
{
    int a;
    int b;
    int res;
} nums;

void add(void *args)
{
    nums *num = (nums *)args;
    int *res = (int *)malloc(sizeof(int));
    printf("a: %d\tb: %d\n", num->a, num->b);
    cthread_yield();
    sleep(3);
    puts("hello");
    cthread_yield();
    *res = num->a + num->b;
    num->res = *res;
    cthread_exit();
}

void loop(void *args)
{
    int i = 5;
    while (i > 0)
    {
        printf("...printing\n");
        sleep(1);
        cthread_yield();
        i--;
    }
    cthread_exit();
}

void func(void *args)
{
    cthread_create(loop, 0);
    cthread_create(add, args);
    cthread_join_all();

    cthread_exit();
}

int main()
{
    int a = 10, b = 12;
    nums num;
    num.a = a;
    num.b = b;

    cthread_init(func, (void *)&num);
    printf("Result is %d\n", num.res);
}