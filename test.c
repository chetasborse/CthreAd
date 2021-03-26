#include<stdio.h>
#include<unistd.h>
#include "cthread.h"

#define SIZESTACK (1024 * 1024)

typedef struct nums {
    int a;
    int b;
}nums;

void handle_usr1(int sig) {
    fflush(stdout);
}

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

int main() {
    struct sigaction sa;
    sa.sa_handler = &handle_usr1;
    sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);

    cthread c1;
    int *result;
    nums num1;
    num1.a = 4;
    num1.b = 4;
    int pid1 = cthread_create(&c1, add, &num1);

    cthread_join(&c1, (void *) &result);
    printf("Result is %d\n", *result);
    return 0;
}