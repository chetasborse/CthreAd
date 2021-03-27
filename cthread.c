#define _GNU_SOURCE
#include<sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <setjmp.h>
#include <sys/wait.h>
#include<signal.h>
#include <linux/futex.h>
#include <syscall.h>
#include "cthread.h"

#define SIZESTACK (1024 * 1024)

void handle_usr1(int sig) {
    fflush(stdout);
}

// Installs signal handler for SIGUSR1 which is used to receive termination signal
void cthread_init() {
    struct sigaction sa;
    sa.sa_handler = &handle_usr1;
    sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
}

// Runs the function passed to the thread
int cthread_run(void *c) {
    cthread *c1 = (cthread *)c;
    if(sigsetjmp(c1->env, 0) == 0) {
        c1->result = c1->func(c1->args);
    }
    kill(c1->ptid, SIGUSR1);
    c1->execution = 1;
    return 0;
}

//Creates a thread for the passed function
int cthread_create(cthread *c, void *(*f)(void *), void *args) {
    char *stack, *stackhead;
    stack = (char *)malloc(SIZESTACK);

    if(!stack) {
		fprintf(stderr, "Unalbe to allocate stack.\n");
		exit(1);
	}

    stackhead = stack + SIZESTACK - 1;

    c->execution = 0;
    c->stack_start = stackhead;
    c->args = args;
    c->func = f;
    c->stack_size = SIZESTACK;
    c->ptid = getpid();
   
    c->tid = clone(cthread_run, stackhead, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SYSVSEM
                       | CLONE_SIGHAND | CLONE_THREAD
                       | CLONE_PARENT_SETTID
                       | CLONE_CHILD_CLEARTID, c);
    
    if(c->tid == -1) {
		fprintf(stderr, "Unable to clone\n");
		free(stack);
		exit(1);
	}   

    return c->tid;
}

//Waits for the thread to complete its execution
int cthread_join(cthread *c, void **result) {
    while(c->execution == 0)
        pause();

    if(result != NULL)
        *result = c->result;

    free((void *)(c->stack_start + 1 - SIZESTACK));
    return 0;
}