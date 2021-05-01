#include <stdio.h>
#include <unistd.h>
#include "../cthread.h"

void *func(void *args)
{
    cthread *c1 = cthread_get_self();
    int tid = c1->tid;
    printf("From thread: Tid => %d\n", tid);
    sleep(1);
    return NULL;
}

int main()
{
    cthread_init();
    cthread c1;
    int pid1 = cthread_create(&c1, func, NULL);
    cthread_join(&c1, NULL);
    printf("Back to main\n");
    printf("From main: Tid => %d\n", pid1);
    return 0;
}