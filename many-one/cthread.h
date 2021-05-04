#define _GNU_SOURCE
#include <setjmp.h>

// maximum number of threads can be altered
#define MAX_THREADS 10

// tcb of thread
typedef struct cthread
{
    int tid;        //thread id
    jmp_buf buffer; // buffer of the running thread
    char *stack;    // stack allocated to the thread

    void *(*function)(void *); // pointer to the routine
    void *args;                // pointer to arguments of the routine
    void *ret_val;             // pointer to the return value of the routine

    int state;     //state of the thread
    int joined_on; //threadId on which this thread is joined on
} cthread;

// states of a thread can be in
typedef enum state
{
    ACTIVE,
    BLOCKED,
    FINISHED,
    INVALID
} state;

// function to initialise the threading library
// pre = 1 for pre-emptive scheduling,
// pre = 0 for non pre-emptive scheduling
void cthread_init(int pre);

// function to create a thread instance
int cthread_create(void *(*func)(void *), void *args);

// function to yield the running thread to the kernel thread
int cthread_yield(void);

// function to wait till the thread tid finishes executing
void cthread_join(int tid);

// function to exit the thread safely
void cthread_exit(void *ret_val);

// definitions for synchronisation

typedef struct cthread_spinlock
{
    int flag; //Tells if the lock is locked=1 or unlocked=0
} cthread_spinlock;

void cthread_spinlock_init(cthread_spinlock *lock);

int cthread_spinlock_lock(cthread_spinlock *sl);

int cthread_spinlock_unlock(cthread_spinlock *sl);

// utility function to print all the threads
void print_queue();