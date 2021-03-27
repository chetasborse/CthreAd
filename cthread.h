#define _GNU_SOURCE
#include<sched.h>
#include <linux/types.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <setjmp.h>

typedef struct cthread {
    int tid; //thread id
    int execution; // 1 if the execution of thread completes, 0 if still executing
    int ptid; //pid of the calling process
    void *stack_start; //address of the start of the stack
    int stack_size;
    void *args; //Pointer pointing to arguments passed to the function
    void *result; //Pointer pointing to return value of the function
    void *(*func)(void *); //Pointer pointing to the address of the function called
    sigjmp_buf env; //Saves current PC and SP when sigsetjmp is called
}cthread;

void cthread_init();
int cthread_run(void *);
int cthread_create(cthread *, void *(*f)(void *), void *);
int cthread_join(cthread *, void **);

void cthread_exit();

int cthread_get_self();

