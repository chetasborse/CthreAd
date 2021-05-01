#include <stdio.h>
#include <unistd.h>
#include "../cthread.h"

typedef struct nums
{
    int a;
    int b;
} nums;

void *add(void *args)
{
    nums *num = (nums *)args;
    int *res = (int *)malloc(sizeof(int));
    printf("From Thread: Numbers are %d and %d\n", num->a, num->b);
    sleep(1);
    printf("From Thread: Addition of numbers is %d\n", num->a + num->b);
    *res = num->a + num->b;
    return res;
}

int main()
{
    cthread_init();
    cthread c1;
    int *result;
    nums num1;
    num1.a = 53;
    num1.b = 82;
    printf("From Main: Numbers are %d and %d\n", num1.a, num1.b);
    int pid1 = cthread_create(&c1, add, &num1);
    cthread_join(&c1, (void *)&result);
    printf("From Main: Result is %d\n", *result);
    return 0;
}