#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cthread.h"

int counter = 0;

void *func(void *args)
{
    while (counter < 4)
    {
        counter++;
        puts("In1");
        sleep(1);
    }

    cthread_exit(NULL);
}
void *func2(void *args)
{
    while (counter < 4)
    {
        counter++;
        puts("In2");
    }
    cthread_exit(NULL);
}

int main()
{
    int t1, t2, t3;
    int x = 0, y = 0;
    cthread_init(1);

    t1 = cthread_create(func, NULL);
    t2 = cthread_create(func2, NULL);

    cthread_join(t1, NULL);
    cthread_join(t2, NULL);

    return 0;
}