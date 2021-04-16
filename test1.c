#include<stdio.h>
#include<unistd.h>
#include "cthread.h"

void *loop(void *args) {
    int i = 4;
    while(i > 0) {
        printf("From thread\n");
        sleep(1);
        i--;
    }
    return NULL;
}

int main() {
    cthread_init();
    cthread c1;
    int pid1 = cthread_create(&c1, loop, NULL);
    cthread_join(&c1, NULL);
    printf("Back to main\n");
    return 0;
}