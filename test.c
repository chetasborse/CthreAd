#include<stdio.h>
#include<unistd.h>
#include "cthread.h"
#include<stdint.h>

typedef struct nums {
    int a;
    int b;
}nums;

int *res;
int counter;
cthread_mutex lock;
cthread_spinlock lock1;

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
    return res;
    //cthread_exit(res);
}

void *count_to_big(void *arg) {
    cthread *c = cthread_get_self();
    for(uint32_t i = 0; i < 10000; i++) {
        cthread_mutex_lock(&lock);
        //cthread_spinlock_lock(&lock1);
        counter++;
        //cthread_spinlock_unlock(&lock1);
        cthread_mutex_unlock(&lock);
    }
}

void *loop(void *args) {
    int i = 10;
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
    //cthread_mutex_init(&lock);
    //cthread_spinlock_init(&lock1);
    cthread c1, c2, c3;
    int *result;
    nums num1;
    num1.a = 4;
    num1.b = 4;
    int pid1 = cthread_create(&c1, count_to_big, NULL);
    int pid2 = cthread_create(&c2, count_to_big, NULL);
    int pid3 = cthread_create(&c3, count_to_big, NULL);
    //int pid = cthread_create(&c1, loop, NULL);

    // cthread_join(&c1, (void *) &result, NULL);
    // cthread_join(&c2, NULL, NULL);

    cthread_join(&c1, NULL, NULL);
    cthread_join(&c2, NULL, NULL);
    cthread_join(&c3, NULL, NULL);
    printf("the value is %d\n", counter);
    //printf("Contents of the lock\nfutex_val: %u\npid: %d\n", lock.futex_val, lock.tid);
    if(lock.flag == 0)
        printf("yass\n");
    else
        printf("lololnononono\n");



        
    
    //sleep(7);
    //printf("Addition is %d\n", *res);

    //sleep(2);
    //printf("From the main thread\n");
    //cthread_exit(NULL);
    

    //printf("Result is %d\n", *result);
    //printf("Execution: %d\n", c1.execution);

    // int pid3 = cthread_create(&c3, dummy, NULL);
    // cthread_join(&c3, NULL, NULL);

    printf("Done\n");

    return 0;
}