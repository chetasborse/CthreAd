#include<stdio.h>
#include<unistd.h>
#include "cthread.h"

typedef struct nums {
    int a;
    int b;
}nums;

int *res;

void *add(void *args) {
    nums *num = (nums *)args;
    //int *res = (int *)malloc(sizeof(int));
    res = (int *)malloc(sizeof(int));
    printf("Addition of numbers is %d\n", num->a+num->b);
    sleep(1);
    printf("Hello %d %d\n", num->a, num->b);
    sleep(1);
    printf("Hello again\n");
    *res = num->a + num->b;
    //return res;
    cthread_exit(res);
}

void *loop(void *args) {
    int i = 5;
    while(i > 0) {
        printf("...printing\n");
        sleep(1);
        i--;
    }
    //return NULL;
    //cthread_exit(NULL);
}

void *dummy(void *args) {
    // int pid = gettid();
    // printf("Tid from function is %d\n", pid);
    printf("Hi there\n");
    return NULL;
}

int main() {
    cthread_init();
    cthread c1, c2, c3;
    int *result;
    nums num1;
    num1.a = 4;
    num1.b = 4;
    //int pid1 = cthread_create(&c1, add, &num1);
    int pid2 = cthread_create(&c2, loop, NULL);

    //cthread_join(&c1, (void *) &result, NULL);
    cthread_join(&c2, NULL, NULL);
    printf("pahila joined called\n");
    cthread_join(&c2, NULL, NULL);
    
    //sleep(7);
    //printf("Addition is %d\n", *res);

    //sleep(2);
    printf("From the main thread\n");
    //cthread_exit(NULL);
    

    //printf("Result is %d\n", *result);
    //printf("Execution: %d\n", c1.execution);

    //int pid3 = cthread_create(&c3, dummy, NULL);
    //cthread_join(&c3, NULL, NULL);

    return 0;
}