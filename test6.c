#include<stdio.h>
#include<unistd.h>
#include "cthread.h"

typedef struct nums {
    int a;
    int b;
}nums;

int res;

void *add(void *args) {
    nums *num = (nums *)args;
    sleep(1);
    res = num->a + num->b;
    sleep(1);
    cthread_exit(&res);
}

int main() {
    cthread_init();
    int i = 4;
    nums num1;
    num1.a = 53;
    num1.b = 82;
    cthread c1;
    printf("From main: Numbers are %d and %d\n", num1.a, num1.b);
    int pid1 = cthread_create(&c1, add, &num1);
    while(i > 0) { //To simulate execution of some other process in main
        sleep(1);
        i--;
    }
    printf("Result of the addition is %d\n", res);
    return 0;
}