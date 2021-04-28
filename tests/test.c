#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../cthread.h"

void *func(void *args)
{
    int counter = 0;

    while (counter < 4)
    {
        counter++;
        puts("In1");
        cthread_yield();
        // sleep(1);
    }

    cthread_exit(NULL);
}
void *func2(void *args)
{
    int counter = 0;

    while (counter < 4)
    {
        counter++;
        puts("In2");
    }
    cthread_exit(NULL);
}

int main()
{
    int t1, t2;
    cthread_init(1);
    t1 = cthread_create(func, NULL);
    t2 = cthread_create(func2, NULL);
    int x = 0;
    while (x < 7)
    {
        puts("main");
        sleep(1);
        x++;
    }

    cthread_join(t1);
    cthread_join(t2);

    return 0;
}