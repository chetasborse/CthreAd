#define _GNU_SOURCE
#include<sched.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <setjmp.h>

// Structure of a thread
typedef struct cthread {
    int tid; //thread id
    int execution; // 1 if the execution of thread completes, 0 if still executing
    int ptid; //pid of the calling process
    int stack_size;
    void *stack_start; //address of the start of the stack
    void *args; //Pointer pointing to arguments passed to the function
    void *result; //Pointer pointing to return value of the function
    void *(*func)(void *); //Pointer pointing to the address of the function called
    u_int32_t futex_val; //Saves the operation to be performed for blocking
    sigjmp_buf env; //Saves current PC and SP when sigsetjmp is called
    struct cthread *next; //Points to next thread in queue
}cthread;

// Queue for threads created
typedef struct queue {
	cthread *head;
	cthread *tail;
}queue;

// Initialisation of queue of threads
queue *threads_queue;

// Thread prototypes start here
void cthread_init();
int cthread_run(void *);
int cthread_create(cthread *, void *(*f)(void *), void *);
int cthread_join(cthread *, void **, struct timespec *);
void cthread_exit(void *);
int cthread_kill(cthread, int);
cthread *cthread_get_self();


// Queue prototypes start here
void enq(queue *, cthread *);
cthread deq(queue *);
int qfull(queue *);
int qempty(queue *);
void traverse(queue *);
cthread* get_details(queue *, int tid);


