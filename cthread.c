#include <ucontext.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "cthread.h"

// variables
int thread_id = 1;
thread_queue init_queue;
thread_queue ready_queue;

cthread *main_thread;
cthread *running_thread;

ucontext_t scheduler_context;
ucontext_t init_context;

int is_qempty(thread_queue *q)
{
    return !q->value;
}

void enqueue(thread_queue *q, cthread *thread)
{
    if (q->tail)
    {
        q->tail->next = thread;
        thread->prev = q->tail;
    }
    else
    {
        q->head = thread;
        thread->prev = NULL;
    }

    q->tail = thread;
    thread->next = NULL;
}

cthread *dequeue(thread_queue *q)
{
    cthread *t = NULL;
    if (q->head)
    {
        t = q->head;
        q->head = t->next;
    }
    if (t->next)
    {
        t->next->prev = NULL;
        t->next = NULL;
    }
    else
    {
        q->tail = NULL;
    }
    return t;
}

// int exists_in_q(thread_queue *q, cthread *thread)
// {
//     if (is_qempty(q))
//         return 0;

//     node *temp = q->head;
//     while (temp)
//     {
//         if (temp->thread == thread)
//             return 1;
//         temp = temp->next;
//     }

//     return 0;
// }

// void delete_thread(thread_queue *q, cthread *thread)
// {
//     if (is_qempty(q))
//     {
//         return;
//     }
//     node *temp = q->head;
//     node *previous;

//     while (temp)
//     {
//         if (thread == temp->thread)
//         {
//             if ((temp == q->head) && (q->head == q->tail))
//             {
//                 q->head = NULL;
//                 q->tail = NULL;
//             }
//             else if (temp == q->head)
//             {
//                 q->head = q->head->next;
//             }
//             else if (temp == q->tail)
//             {
//                 q->tail = previous;
//                 q->tail->next = NULL;
//             }
//             else
//             {
//                 previous->next = temp->next;
//             }
//             return;
//         }
//         previous = temp;
//         temp = temp->next;
//     }
//     return;
// }

// thread functions

void cthread_init(void (*func)(void *), void *args)
{
    main_thread = setup_thread(func, args);
    enqueue(&ready_queue, main_thread);
    thread_scheduler();
}

cthread *setup_thread(void (*func)(void *), void *args)
{
    cthread *t = calloc(1, sizeof(cthread));
    if (!t)
    {
        perror("Malloc failed in setup_thread!\n");
        exit(1);
    }

    if (getcontext(&t->context) == -1)
    {
        perror("getcontext failed\n");
    }
    t->context.uc_stack.ss_sp = calloc(1, SIGSTKSZ);
    if (!t->context.uc_stack.ss_sp)
    {
        perror("Calloc failed\n");
        exit(1);
    }
    t->context.uc_stack.ss_size = SIGSTKSZ;
    t->context.uc_link = NULL;

    makecontext(&t->context, (void (*)())func, 1, args);

    return t;
}

cthread cthread_create(void (*func)(void *), void *args)
{
    cthread *t = setup_thread(func, args);
    t->tid = thread_id++;
    t->parent = running_thread;
    t->child_count = 0;
    t->child_spot = running_thread->child_count;
    t->blocked = 0;
    t->join_child = NULL;

    running_thread->children_list[running_thread->child_count++] = t;

    // Put thread in ready queue
    enqueue(&ready_queue, t);
    return *t;
}

void cthread_yield(void)
{
    if (ready_queue.head)
    {
        enqueue(&ready_queue, running_thread);
        running_thread = dequeue(&ready_queue);
        swapcontext(&ready_queue.tail->context, &running_thread->context);
    }
    else
    {
        // pass
    }
}

int cthread_join(cthread *thread)
{
    cthread *t = thread;

    if (t->exit)
    {
        return -1;
    }

    int i = 0;
    char is_child = 0;
    for (i = 0; i < running_thread->child_count; i++)
    {
        if (running_thread->children_list[i]->tid == t->tid)
        {
            is_child = 1;
        }
    }

    if (!is_child)
    {
        return -1;
    }

    running_thread->blocked = 1;
    running_thread->join_child = t;

    swapcontext(&running_thread->context, &scheduler_context);

    return 0;
}

void cthread_join_all(void)
{
    if (running_thread->child_count == 0)
    {
        return;
    }

    running_thread->blocked = 1;
    swapcontext(&running_thread->context, &scheduler_context);

    return;
}

void cthread_exit(void)
{
    // Handle any blocked parent
    cthread *t = running_thread->parent;
    if (t)
    {
        if (t->blocked)
        {
            if (t->join_child)
            { // If join, not join all
                if (t->join_child->tid == running_thread->tid)
                {
                    t->blocked = 0;
                    t->join_child = NULL;
                    enqueue(&ready_queue, t);
                }
            }
            else
            { // If join all
                if (t->child_count == 1)
                {
                    t->blocked = 0;
                    enqueue(&ready_queue, t);
                }
            }
        }
        // Update children status in the parent thread
        int temp = running_thread->child_spot;
        t->children_list[temp] = t->children_list[t->child_count - 1];
        t->children_list[temp]->child_spot = temp;
        t->child_count--;
    }
    // Null all the parent pointer of its children
    int i;
    for (i = 0; i < running_thread->child_count; i++)
    {
        running_thread->children_list[i]->parent = NULL;
    }
    // Set exit tag so the engine knows to stop thread
    running_thread->exit = 1;
    swapcontext(&running_thread->context, &scheduler_context);
}

// scheduler
int thread_scheduler(void)
{
    while (1)
    {
        cthread *t;
        if (ready_queue.head == NULL)
        {
            return 0;
        }
        t = dequeue(&ready_queue);

        running_thread = t;
        swapcontext(&scheduler_context, &running_thread->context);

        t = running_thread;
        if (t->exit)
        {
            free(t->context.uc_stack.ss_sp);
            free(t);
            t = NULL;
        }
    }
}