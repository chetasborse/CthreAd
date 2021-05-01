#define _GNU_SOURCE
#include <setjmp.h>
#define MAX_THREADS 10
// tcb of thread
typedef struct cthread
{
    int tid; //thread id
    jmp_buf buffer;
    char *stack;

    void *(*function)(void *);
    void *args;
    void *ret_val;

    int state;
    int joined_on;
} cthread;

// queue structure for thread pool
typedef struct thread_queue
{
    cthread *head;
    cthread *tail;
    int value;
} thread_queue;

// states of any thread
typedef enum state
{
    ACTIVE,
    BLOCKED,
    FINISHED,
    INVALID
} state;

// structure for scheduler
int thread_scheduler();

void cthread_init(int);

int cthread_create(void *(*func)(void *), void *args);

int cthread_yield(void);

void cthread_join(int tid);

void cthread_exit(void *ret_val);

// defs for synchronisation

typedef struct cthread_spinlock
{
    int flag; //Tells if the lock is locked=1 or unlocked=0
} cthread_spinlock;

void cthread_spinlock_init(cthread_spinlock *lock);

int cthread_spinlock_lock(cthread_spinlock *sl);

int cthread_spinlock_unlock(cthread_spinlock *sl);

void print_queue();