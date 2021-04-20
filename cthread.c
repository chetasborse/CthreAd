#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <signal.h>
#include <linux/futex.h>
#include <syscall.h>
#include "cthread.h"

#define SIZESTACK (1024 * 1024)

// Pushes a thread to the queue
static void enq(cthread *d) {
    queue *q = threads_queue;
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
static void traverse() {
    queue *q = threads_queue;
    cthread *ptr;
    ptr = q->head;
    do {
        if(ptr->execution == 1)
            printf("Thread id: %d\n", ptr->tid);
        ptr = ptr->next;
    }while(ptr != q->head);
}

// Checks if the queue is empty
static int qempty() {
	return (threads_queue->head == NULL);
}

// System call to halt parent till the value in futex_val doesn't change to 0
static int futex_wait(void* addr, int tid){
  return syscall(SYS_futex,addr,FUTEX_WAIT, tid, NULL, NULL, 0);
}

// Wakes only one process waiting in futex
static int futex_wake(void* addr){
  return syscall(SYS_futex,addr,FUTEX_WAKE, 1, NULL, NULL, 0);
}

// System call to send signal to the intended thread
static int thread_kill(int tid, int signal) {
    return syscall(SYS_tgkill, getpid(), tid, signal);
}

// Sets number in the passed address to 1 and returns the previous value
static int test_and_set(int *flag) {
    int result, set = 1;
    asm("lock xchgl %0, %1" :
        "+m" (*flag), "=a" (result):
        "1" (set) :
        "cc");
    return result;
}

// Returns pointer to the thread struct with the given thread_id
cthread *get_details(int tid) {
    queue *q = threads_queue;
    cthread *ptr;
    ptr = q->head;
    do {
        if(ptr->tid == tid)
            return ptr;
        ptr = ptr->next;
    }while(ptr != q->head);
    return NULL;
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
    enq(c1);
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
        printf("Unable to create thread\n");
		exit(1);
	}   

    return c->tid;
}

// Waits for the thread to complete its execution
int cthread_join(cthread *c, void **result) {
    
    if(c->execution == 1)
        return 0;
        
    futex_wait(&c->futex_val, c->tid);

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
    cthread *c1 = get_details(tid);
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

// Checks if 2 threads are equal. Returns non-zero number if equal
int cthread_equal(cthread c1, cthread c2) {
    if(c1.tid == c2.tid)
        return 1;
    return 0;
}

// Initializes flag in mutex lock to 0
void cthread_mutex_init(cthread_mutex *lock) {
    lock->flag = 0;
}

// Locks the critical section in the thread using sleeplock
int cthread_mutex_lock(cthread_mutex *cm) {
    while(1) {
        int flag_status = test_and_set(&cm->flag);
        if(flag_status == 0) {
            break;
        }
        futex_wait(&cm->flag, 1);
    }
    return 1;
}

// Unlocks the critical section of the thread using sleeplock
int cthread_mutex_unlock(cthread_mutex *cm) {
    cm->flag = 0;
    futex_wake(&cm->flag);
    return 1;
}

// Initializes flag in spinlock to 0
void cthread_spinlock_init(cthread_spinlock *lock) {
    lock->flag = 0;
}

// Locks the critical section in the thread using spinlock
int cthread_spinlock_lock(cthread_spinlock *sl) {
    while(1) {
        int flag_status = test_and_set(&sl->flag);
        if(flag_status == 0) {
            break;
        }
    }
    return 1;
}

// Unlocks the critical section of the thread using spinlock
int cthread_spinlock_unlock(cthread_spinlock *sl) {
    sl->flag = 0;
    return 1;
}


