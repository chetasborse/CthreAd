#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include "cthread.h"

int thread_id = 1;

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

cthread *running;
cthread *main;
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

thread_queue *ready;
thread_queue *blocked;

void cthread_init()
