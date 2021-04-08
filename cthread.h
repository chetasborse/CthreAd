#define _GNU_SOURCE
#include <sched.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <setjmp.h>
#include <ucontext.h>

// tcb of thread
typedef struct cthread
{
    int tid;                           //thread id
    ucontext_t context;                //context of the thread
    struct cthread *next;              // pointer to next thread
    struct cthread *prev;              // pointer to previous thread
    struct cthread *parent;            // pointer to parent thread
    struct cthread *children_list[10]; // pointer to list of child threads
    int child_count;                   //number of child threads
    int child_spot;
    char blocked;               // indicate if a thread is blocked
    struct cthread *join_child; //join a child thread
    char exit;
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
    READY,
    RUNNING,
    SCHEDULED,
    WAITING,
    TERMINATED,
    BLOCKED,
} state;

// structure for scheduler
int thread_scheduler();

typedef struct Semaphore
{
    int val;
    thread_queue *sem_queue;
} Semaphore;

void cthread_init(void (*func)(void *), void *args);
int cthread_run(void *);
void cthread_yield(void);
cthread cthread_create(void (*func)(void *), void *args);
cthread *setup_thread(void (*func)(void *), void *args);
int cthread_join(cthread *);
void cthread_join_all(void);
void cthread_exit();

int cthread_get_self();
