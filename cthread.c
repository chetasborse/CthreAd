#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include "cthread.h"

// you have ignored blocked on by parent thread

int thread_count = 1;

thread_queue *q_init()
{
    thread_queue *q;
    q = (thread_queue *)malloc(sizeof(thread_queue));
    if (!q)
    {
        return NULL;
    }

    return q;
}

int is_qempty(thread_queue *q)
{
    return !q->size;
}

cthread *running_thread;
cthread *main_thread;
ucontext_t *main_context;

void enqueue(thread_queue *q, cthread *thread)
{
    node *new = (node *)malloc(sizeof(node));
    new->thread = thread;
    new->next = NULL;

    if (q->tail != NULL)
        q->tail->next = new;

    q->tail = new;

    if (q->head == NULL)
        q->head = q->tail;
}

cthread *dequeue(thread_queue *q)
{
    if (q->head == NULL)
        return NULL;

    cthread *temp = q->head->thread;
    if (q->head == q->tail)
        q->head = q->tail = NULL;

    else
        q->head = q->head->next;

    return temp;
}

int exists_in_q(thread_queue *q, cthread *thread)
{
    if (is_qempty(q))
        return 0;

    node *temp = q->head;
    while (temp)
    {
        if (temp->thread == thread)
            return 1;
        temp = temp->next;
    }

    return 0;
}

void delete_thread(thread_queue *q, cthread *thread)
{
    if (is_qempty(q))
    {
        return;
    }
    node *temp = q->head;
    node *previous;

    while (temp)
    {
        if (thread == temp->thread)
        {
            if ((temp == q->head) && (q->head == q->tail))
            {
                q->head = NULL;
                q->tail = NULL;
            }
            else if (temp == q->head)
            {
                q->head = q->head->next;
            }
            else if (temp == q->tail)
            {
                q->tail = previous;
                q->tail->next = NULL;
            }
            else
            {
                previous->next = temp->next;
            }
            return;
        }
        previous = temp;
        temp = temp->next;
    }
    return;
}

thread_queue *ready_queue;
thread_queue *blocked_queue;

void cthread_init(void (*func)(void *), void *args)
{
    cthread *thread = (cthread *)malloc(sizeof(cthread));
    thread->tid = thread_count;
    thread_count++;

    thread->state = READY;
    thread->parent = NULL;
    thread->child_queue = q_init();

    ready_queue = q_init();
    blocked_queue = q_init();

    getcontext(&thread->context);
    thread->context.uc_stack.ss_sp = malloc(1024 * 8);
    thread->context.uc_stack.ss_size = 1024 * 8;
    makecontext(&thread->context, (void (*)(void))func, 1, args);

    running_thread = thread;
    main_thread = thread;

    main_context = (ucontext_t *)malloc(sizeof(ucontext_t));
    swapcontext(main_context, &thread->context);
}

void *cthread_create(void (*func)(void *), void *args)
{
    cthread *thread = (cthread *)malloc(sizeof(cthread));
    thread->tid = thread_count;
    thread_count++;

    thread->state = READY;
    thread->parent = running_thread;
    thread->child_queue = q_init();

    getcontext(&thread->context);
    thread->context.uc_stack.ss_sp = malloc(1024 * 8);
    thread->context.uc_stack.ss_size = 1024 * 8;
    makecontext(&thread->context, (void (*)(void))func, 1, args);

    enqueue(ready_queue, thread);
    enqueue(running_thread->child_queue, thread);

    return (void *)thread;
}

void cthread_yield(void)
{
    cthread *temp = running_thread;
    enqueue(ready_queue, running_thread);

    cthread *temp2 = dequeue(ready_queue);
    if (temp2 != NULL)
    {
        running_thread = temp2;
        swapcontext(&temp->context, &temp2->context);
    }
    else
    {
        setcontext(main_context);
    }
}

int cthread_join(void *thread)
{
    cthread *child_thread = (cthread *)thread;

    if (child_thread->state != TERMINATED)
    {
        if (child_thread->parent == running_thread)
        {
            cthread *temp = running_thread;
            enqueue(blocked_queue, running_thread);
            cthread *temp2 = dequeue(ready_queue);
            if (temp2 != NULL)
            {
                running_thread = temp2;
                swapcontext(&temp->context, &temp2->context);
            }
            else
            {
                setcontext(main_context);
            }
            return 0;
        }
        else
            return -1;
    }

    return 0;
}