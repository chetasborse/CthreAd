#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <stdatomic.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "cthread.h"

#define STACK_SIZE 16 * 4096
#define MAIN_THREAD 0

cthread thread_context[MAX_THREADS];
cthread *running_thread;
cthread *new_thread = NULL;

char stack[SIGSTKSZ * (MAX_THREADS + 1)];

int thread_id = 0;
int created = 0;
int main_created = 0;
int preempt = 0;
int thread_count = 0;

struct itimerval start_timer;
struct itimerval zero_timer;

void sigusr_handler(int sig)
{

    if (setjmp(new_thread->buffer) == 0)
    {
        created = 1;
    }
    else
    {
        if (preempt)
        {
            ualarm(25000, 0);
        }
        running_thread->function(running_thread->args);
    }
}

void sigalarm_handler(int sig)
{
    cthread_yield();
}

void cthread_init(int pre)
{
    for (int i = 0; i < MAX_THREADS; i++)
    {
        cthread new;
        new.function = NULL;
        new.args = NULL;
        new.ret_val = NULL;
        if (i == 0)
            new.state = ACTIVE;
        else
            new.state = INVALID;

        new.tid = i;
        new.stack = stack + (i * SIGSTKSZ);

        thread_context[i] = new;
    }

    running_thread = &thread_context[MAIN_THREAD];

    // yield
    struct sigaction sa_usr;
    memset(&sa_usr, 0, sizeof(struct sigaction));
    sa_usr.sa_handler = sigusr_handler;
    sa_usr.sa_flags = SA_ONSTACK;
    sigemptyset(&sa_usr.sa_mask);

    if (sigaction(SIGUSR1, &sa_usr, NULL) == -1)
    {
        printf("Error in sigaction/SIGUSR\n");
        exit(1);
    }

    // alarm
    if (pre)
    {
        preempt = 1;
        signal(SIGALRM, sigalarm_handler);
        ualarm(25000, 0);
    }
}

int cthread_create(void *(*func)(void *), void *args)
{
    for (int i = 1; i < MAX_THREADS; i++)
    {
        if (thread_context[i].state == INVALID)
        {
            thread_context[i].function = func;
            thread_context[i].args = args;
            thread_context[i].state = ACTIVE;
            thread_context[i].joined_on = -1;

            stack_t new;
            new.ss_flags = 0;
            new.ss_size = SIGSTKSZ;
            new.ss_sp = thread_context[i].stack;

            if (sigaltstack(&new, NULL) == -1)
            {
                printf("Error/SIGALTSTACK\n");
                exit(1);
            }

            new_thread = &thread_context[i];
            raise(SIGUSR1);

            while (!created)
            {
            };

            created = 0;
            return i;
        }
    }
    return -1;
}

int cthread_yield(void)
{
    ualarm(0, 0);
    if (preempt)
    {
        setitimer(ITIMER_VIRTUAL, &zero_timer, NULL);
    }
    int curr;

    for (int i = 1; i <= MAX_THREADS; i++)
    {
        curr = (i + running_thread->tid) % MAX_THREADS;

        if (thread_context[curr].state == ACTIVE)
        {
            if (setjmp(running_thread->buffer) == 0)
            {
                running_thread = &thread_context[curr];
                longjmp(running_thread->buffer, 1);
            }
            else
            {
                if (preempt)
                {
                    ualarm(25000, 0);
                }
            }
            return 1;
        }
    }

    return 0;
}

void cthread_join(int tid)
{
    ualarm(0, 0);
    running_thread->state = BLOCKED;
    thread_context[tid].joined_on = running_thread->tid;

    if (setjmp(running_thread->buffer) == 0)
    {
        running_thread = &thread_context[tid];
        ualarm(25000, 0);
        longjmp(running_thread->buffer, 1);
    }
    else
    {
        if (preempt)
            ualarm(25000, 0);
    }
}

void cthread_exit(void *ret_val)
{
    running_thread->state = FINISHED;
    running_thread->ret_val = ret_val;

    if (running_thread->joined_on != -1)
    {
        running_thread = &thread_context[running_thread->joined_on];
        running_thread->state = ACTIVE;
        longjmp(running_thread->buffer, 1);
    }
    cthread_yield();
}

// synchronisation functions

// Sets number in the passed address to 1 and returns the previous value
static int test_and_set(int *flag)
{
    int result, set = 1;
    asm("lock xchgl %0, %1"
        : "+m"(*flag), "=a"(result)
        : "1"(set)
        : "cc");
    return result;
}

void cthread_spinlock_init(cthread_spinlock *lock)
{
    lock->flag = 0;
}

int cthread_spinlock_lock(cthread_spinlock *sl)
{
    while (1)
    {
        int flag_status = test_and_set(&sl->flag);
        if (flag_status == 0)
        {
            break;
        }
    }
    return 1;
}

int cthread_spinlock_unlock(cthread_spinlock *sl)
{
    sl->flag = 0;
    return 1;
}

// utility function to print queue
void print_queue()
{
    printf("----------x---------\n");
    for (int i = 0; i < MAX_THREADS; i++)
    {
        printf("TID: %d\n", thread_context[i].tid);
        printf("State: %d\n", thread_context[i].state);
        printf("Waiting on: %d\n", thread_context[i].joined_on);
        printf("\n");
    }
    printf("----------x---------\n");
}