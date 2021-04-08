#define _GNU_SOURCE
#include<sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <setjmp.h>
#include <sys/wait.h>
#include<signal.h>
#include <linux/futex.h>
#include <syscall.h>
#include "cthread.h"

#define SIZESTACK (1024 * 1024)

// Pushes a thread to the queue
void enq(queue *q, cthread *d) {
	if(q->head == NULL) {
		q->head = d;
		q->tail = d;
	}
	else {
		(q->tail)->next = d;
		q->tail = d;
	}
	q->tail->next = q->head;
}

// Prints all the running threads
void traverse(queue *q) {
    cthread *ptr;
    ptr = q->head;
    do {
        if(ptr->execution == 1)
            printf("Thread id: %d\n", ptr->tid);
        ptr = ptr->next;
    }while(ptr != q->head);
}

// Checks if the queue is full
int qfull(queue *q) {
	return 0;
}

// Checks if the queue is empty
int qempty(queue *q) {
	return (q->head == NULL);
}

// Returns pointer to the thread struct with the given thread_id
cthread *get_details(queue *q, int tid) {
    cthread *ptr;
    ptr = q->head;
    do {
        if(ptr->tid == tid)
            return ptr;
        ptr = ptr->next;
    }while(ptr != q->head);
    return NULL;
}


// System call to halt parent till the value in futex_val doesn't change to 0
int futex_wait(void* addr, int tid, struct timespec *time){
  return syscall(SYS_futex,addr,FUTEX_WAIT, tid, NULL, NULL, 0);
}

// System call to send signal to the intended thread
int thread_kill(int tid, int signal) {
    return syscall(SYS_tgkill, getpid(), tid, signal);
}

// Initialisation of threads
void cthread_init() {
    threads_queue = (queue *)malloc(sizeof(queue));
    threads_queue->head = NULL;
    threads_queue->tail = NULL;
}

// Runs the function passed to the thread
int cthread_run(void *c) {
    cthread *c1 = (cthread *)c;
    enq(threads_queue, c1);
    if(setjmp(c1->env) == 0) {
        c1->result = c1->func(c1->args);
    }
    else {
        printf("Thread %d exited safely\n", c1->tid);
    }
    c1->execution = 1;
    return 0;
}

// Creates a thread for the passed function
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
    c->next = NULL;
   
    c->tid = clone(cthread_run, stackhead, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SYSVSEM
                       | CLONE_SIGHAND | CLONE_THREAD
                       | CLONE_PARENT_SETTID
                       | CLONE_CHILD_CLEARTID, c, &c->futex_val, c, &c->futex_val);


    if(c->tid == -1) {
		fprintf(stderr, "Unable to clone\n");
		free(stack);
		exit(1);
	}   

    return c->tid;
}

// Waits for the thread to complete its execution
int cthread_join(cthread *c, void **result, struct timespec *time) {
    
    if(c->execution == 1)
        return 0;
        
    futex_wait(&c->futex_val, c->tid, time);

    if(result != NULL)
        *result = c->result;

    free((void *)(c->stack_start + 1 - SIZESTACK));
    return 0;
}

// Gets the thread details of the current thread
cthread *cthread_get_self() {
    int tid = gettid();
    if(tid == getpid())
        return NULL;
    cthread *c1 = get_details(threads_queue, tid);
    return c1;
}

// Stops the execution of the passed thread
void cthread_exit(void *result) {
    cthread *c1 = cthread_get_self();
    c1->result = result;
    if(c1)
        longjmp(c1->env, 1);
    return;
}

// Sends signal to the intended thread
int cthread_kill(cthread c1, int signal) {
    return thread_kill(c1.tid, signal);
}


