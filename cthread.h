#define _GNU_SOURCE
#include <sched.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <setjmp.h>
#include <ucontext.h>

// node in a queue
typedef struct node
{
    struct cthread *thread;
    struct node *next;
} node;

// queue structure for thread pool
typedef struct thread_queue
{
    node *head;
    node *tail;
    int size;
} thread_queue;

// tcb of thread
typedef struct cthread
{
    int tid;                   //thread id
    int execution;             // 1 if the execution of thread completes, 0 if still executing
    int ptid;                  //pid of the calling process
    void *stack_start;         //address of the start of the stack
    int stack_size;            // size of the stack
    void *args;                //Pointer pointing to arguments passed to the function
    void *result;              //Pointer pointing to return value of the function
    void *(*func)(void *);     //Pointer pointing to the address of the function called
    sigjmp_buf env;            //Saves current PC and SP when sigsetjmp is called
    int state;                 // state of the thread
    ucontext_t context;        // context of thread
    thread_queue *child_queue; // points to child queue
    struct cthread *parent;    //pointer to parent thread

} cthread;

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
typedef struct scheduler
{
    thread_queue *main_queue;
    thread_queue *wait;
    cthread *main_thread;
    cthread *current;

    int priority_list[5];
    int count;
} scheduler;

typedef struct Semaphore
{
    int val;
    thread_queue *sem_queue;
} Semaphore;

void cthread_init();
int cthread_run(void *);
void cthread_yield(void);
void *cthread_create(void (*func)(void *), void *args);
int cthread_join(void *);

void cthread_exit();

int cthread_get_self();
