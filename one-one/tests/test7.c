#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include "../cthread.h"
#include <unistd.h>

struct sigaction sa1;

void handle_sigusr1(int sig)
{
    fflush(stdout);
    printf("Signal received by thread %d\n", gettid());
}

void *func(void *args)
{
    int i = 5;
    while (i > 0)
    {
        sleep(1);
        i--;
    }
    return NULL;
}

int main()
{
    sa1.sa_handler = &handle_sigusr1;
    sigemptyset(&sa1.sa_mask);
    sigaction(SIGUSR1, &sa1, NULL);

    cthread_init();
    cthread c1;
    printf("Starting thread\n");
    int pid1 = cthread_create(&c1, func, NULL);
    sleep(1);
    printf("Sending usr1 signal to %d\n", pid1);
    sleep(1);
    cthread_kill(c1, SIGUSR1);
    cthread_join(&c1, NULL);
    printf("Back to main\n");
    return 0;
}