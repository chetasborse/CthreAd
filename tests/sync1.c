#include <stdio.h>
#include <unistd.h>
#include "../cthread.h"

int counter, limit = 100000;
cthread_spinlock lock;

void *count_to_big(void *arg)
{
    for (int i = 0; i < limit; i++)
    {
        cthread_spinlock_lock(&lock);
        counter++;
        cthread_spinlock_unlock(&lock);
    }
    cthread_exit(NULL);
}

int main()
{
    cthread_init(1);
    cthread_spinlock_init(&lock);
    int pid1 = cthread_create(count_to_big, NULL);
    int pid2 = cthread_create(count_to_big, NULL);
    int pid3 = cthread_create(count_to_big, NULL);
    cthread_join(pid1);
    cthread_join(pid2);
    cthread_join(pid3);
    printf("%d\n", counter);
    return 0;
}
