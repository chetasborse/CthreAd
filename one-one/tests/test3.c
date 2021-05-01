#include<stdio.h>
#include<unistd.h>
#include "../cthread.h"

int counter, limit = 1000000;
cthread_mutex lock;

void *count_to_big(void *arg) {
    cthread *c = cthread_get_self();
    for(int i = 0; i < limit; i++) {
        cthread_mutex_lock(&lock);
        counter++;
        cthread_mutex_unlock(&lock);
    }
}

int main() {
    cthread_init();
    cthread_mutex_init(&lock);
    cthread c1, c2, c3;
    int pid1 = cthread_create(&c1, count_to_big, NULL);
    int pid2 = cthread_create(&c2, count_to_big, NULL);
    int pid3 = cthread_create(&c3, count_to_big, NULL);
    printf("Expected counter value: %d\n", 3 * limit);
    cthread_join(&c1, NULL);
    cthread_join(&c2, NULL);
    cthread_join(&c3, NULL);
    printf("Counter value after execution: %d\n", counter);
    return 0;
}