#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "cthread.h"

#define STACK_SIZE 16 * 4096
#define MAIN_THREAD 0

cthread thread_context[MAX_THREADS];
cthread *running_thread;
cthread *new_thread = NULL;

char stack[STACK_SIZE * MAX_THREADS];

int thread_id = 0;
int created = 0;
int preempt = 0;

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
            setitimer(ITIMER_VIRTUAL, &start_timer, NULL);
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
        if (i == 0)
        {
            new.function = NULL;
            new.args = NULL;
            new.ret_val = NULL;
            new.state = ACTIVE;
        }

        new.tid = i;
        new.stack = stack + (i * STACK_SIZE);
        new.state = INVALID;

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
        struct sigaction sa_alarm;
        memset(&sa_alarm, 0, sizeof(struct sigaction));
        sa_alarm.sa_handler = sigalarm_handler;
        sigemptyset(&sa_alarm.sa_mask);

        if (sigaction(SIGVTALRM, &sa_alarm, NULL) == -1)
        {
            printf("Error in sigaction/SIGALARM\n");
            exit(1);
        }

        preempt = pre;

        start_timer.it_interval.tv_sec = 0;
        start_timer.it_interval.tv_usec = 25000;
        start_timer.it_value.tv_sec = 0;
        start_timer.it_value.tv_usec = 25000;

        zero_timer.it_interval.tv_sec = 0;
        zero_timer.it_interval.tv_usec = 0;
        zero_timer.it_value.tv_sec = 0;
        zero_timer.it_value.tv_usec = 0;

        setitimer(ITIMER_VIRTUAL, &start_timer, NULL);
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
            new.ss_size = STACK_SIZE;
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
                    setitimer(ITIMER_VIRTUAL, &start_timer, NULL);
                }
            }
            return 1;
        }
    }

    return 0;
}

void cthread_join(int tid)
{
    running_thread->state = BLOCKED;
    thread_context[tid].joined_on = running_thread->tid;

    if (setjmp(running_thread->buffer) == 0)
    {
        running_thread = &thread_context[tid];
        setitimer(ITIMER_VIRTUAL, &start_timer, NULL);
        longjmp(running_thread->buffer, 1);
    }
    else
    {
        if (preempt)
            setitimer(ITIMER_VIRTUAL, &start_timer, NULL);
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
        setitimer(ITIMER_VIRTUAL, &start_timer, NULL);
        longjmp(running_thread->buffer, 1);
    }
    setitimer(ITIMER_VIRTUAL, &start_timer, NULL);
    cthread_yield();
}