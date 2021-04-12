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

typedef struct cthread_mutex {
    int flag; //Tells if the lock is locked=1 or unlocked=0
}cthread_mutex;

typedef struct cthread_spinlock {
    int flag; //Tells if the lock is locked=1 or unlocked=0
}cthread_spinlock;

// Thread prototypes start here
void cthread_init();
int cthread_run(void *);
int cthread_create(cthread *, void *(*f)(void *), void *);
// int cthread_join(cthread *, void **, struct timespec *);
int cthread_join(cthread *, void **);
void cthread_exit(void *);
int cthread_kill(cthread, int);
cthread *cthread_get_self();


// Queue prototypes start here
void enq(cthread *);
int qempty();
void traverse();
cthread* get_details(int tid);

// Mutex lock prototypes
void cthread_mutex_init(cthread_mutex *);
int cthread_mutex_lock(cthread_mutex *);
int cthread_mutex_unlock(cthread_mutex *);

// Spinlock prototypes
void cthread_spinlock_init(cthread_spinlock *);
int cthread_spinlock_lock(cthread_spinlock *);
int cthread_spinlock_unlock(cthread_spinlock *);

