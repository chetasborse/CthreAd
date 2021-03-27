#include<stdio.h>
#include<unistd.h>
#include "cthread.h"

typedef struct nums {
    int a;
    int b;
}nums;

void *add(void *args) {
    nums *num = (nums *)args;
    int *res = (int *)malloc(sizeof(int));
    printf("Addition of numbers is %d\n", num->a+num->b);
    sleep(1);
    printf("Hello %d %d\n", num->a, num->b);
    sleep(1);
    printf("Hello again\n");
    *res = num->a + num->b;
    return res;
}

void *loop(void *args) {
    int i = 5;
    while(i > 0) {
        printf("...printing\n");
        sleep(1);
        i--;
    }
    return NULL;
}

int main() {

    cthread_init();
    cthread c1, c2;
    int *result;
    nums num1;
    num1.a = 4;
    num1.b = 4;
    int pid1 = cthread_create(&c1, add, &num1);
    int pid2 = cthread_create(&c2, loop, NULL);

    cthread_join(&c1, (void *) &result);
    cthread_join(&c2, NULL);    

    printf("Result is %d\n", *result);
    return 0;
}