#include <stdio.h>
#include "../cthread.h"

static int cnt = 1;

void *thread(void *arg)
{
    cnt *= 2;
    cthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int tids[MAX_THREADS];
    int i;

    cthread_init(1);

    for (i = 0; i < MAX_THREADS - 1; ++i)
    {
        tids[i] = cthread_create(thread, NULL);
    }

    for (i = 0; i < MAX_THREADS - 1; ++i)
    {
        cthread_join(tids[i]);
    }

    printf("%d\n", cnt);

    return 0;
}