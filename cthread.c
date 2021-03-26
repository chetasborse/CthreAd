#define _GNU_SOURCE
#include<sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <setjmp.h>
#include <sys/wait.h>
#include<signal.h>
// #include <sys/mman.h>
// #include <linux/signal.h>
// #include <linux/unistd.h>
// #include <linux/types.h>
// #include <values.h>
#include "cthread.h"

#define SIZESTACK (1024 * 1024)

int cthread_run(void *c) {
    cthread *c1 = (cthread *)c;
    if(sigsetjmp(c1->env, 0) == 0) {
        c1->result = c1->func(c1->args);
    }
    kill(c1->ptid, SIGUSR1);
    return 0;
}

int cthread_create(cthread *c, void *(*f)(void *), void *args) {
    char *stack, *stackhead;
    stack = (char *)malloc(SIZESTACK);

    if(!stack) {
		fprintf(stderr, "Unalbe to allocate stack.\n");
		exit(1);
	}

    stackhead = stack + SIZESTACK - 1;

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

int cthread_join(cthread *c, void **result) {
    int status;
    printf("Waiting\n");
    pause();
    if(result != NULL)
        *result = c->result;
    return 0;
}