#include "../cthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct arg_struct
{
    int ***a1;
    int ***b1;
    int ***c1;
    int r1;
    int c2;
    int r2;
} arg_struct;

void *mat_mul(void *args)
{
    int i, j, k, sum = 0;
    arg_struct *argss = (arg_struct *)args;
    int **a, **b, **c;
    a = *(*argss).a1;
    b = *(*argss).b1;
    c = *(*argss).c1;
    for (i = 0; i < (*argss).r1; i = i + 3)
    {
        for (j = 0; j < (*argss).c2; j++)
        {
            sum = 0;
            for (k = 0; k < (*argss).r2; k++)
                sum += a[i][k] * b[k][j];
            c[i][j] = sum;
        }
    }
    return NULL;
}

void *mat_mul1(void *args)
{
    int i, j, k, sum = 0;
    arg_struct *argss = (arg_struct *)args;
    int **a, **b, **c;
    a = *(*argss).a1;
    b = *(*argss).b1;
    c = *(*argss).c1;
    for (i = 1; i < (*argss).r1; i = i + 3)
    {
        for (j = 0; j < (*argss).c2; j++)
        {
            sum = 0;
            for (k = 0; k < (*argss).r2; k++)
                sum += a[i][k] * b[k][j];
            c[i][j] = sum;
        }
    }
    return NULL;
}

void *mat_mul2(void *args)
{
    int i, j, k, sum = 0;
    arg_struct *argss = (arg_struct *)args;
    int **a, **b, **c;
    a = *(*argss).a1;
    b = *(*argss).b1;
    c = *(*argss).c1;
    for (i = 2; i < (*argss).r1; i = i + 3)
    {
        for (j = 0; j < (*argss).c2; j++)
        {
            sum = 0;
            for (k = 0; k < (*argss).r2; k++)
                sum += a[i][k] * b[k][j];
            c[i][j] = sum;
        }
    }
    return NULL;
}

int main()
{
    cthread_init();
    cthread thread1, thread2, thread3;
    int **a, **b, **c, r1, c1, r2, c2, i, j, k, sum = 0;
    arg_struct args;

    scanf("%d%d", &r1, &c1);
    a = (int **)malloc(sizeof(int *) * r1);
    for (i = 0; i < r1; i++)
    {
        a[i] = (int *)malloc(sizeof(int) * c1);
        for (j = 0; j < c1; j++)
        {
            scanf("%d", &a[i][j]);
        }
    }

    scanf("%d%d", &r2, &c2);
    if (c1 != r2)
    {
        printf("Columns of first matrix should be equal to the rows of the second matrix\n");
        return 1;
    }
    else
    {
        c = (int **)malloc(sizeof(int *) * r1);
        for (i = 0; i < r1; i++)
        {
            c[i] = (int *)malloc(sizeof(int) * c2);
        }
    }
    b = (int **)malloc(sizeof(int *) * r2);
    for (i = 0; i < r2; i++)
    {
        b[i] = (int *)malloc(sizeof(int) * c2);
        for (j = 0; j < c2; j++)
        {
            scanf("%d", &b[i][j]);
        }
    }

    args.a1 = &a;
    args.b1 = &b;
    args.c1 = &c;
    args.r1 = r1;
    args.c2 = c2;
    args.r2 = r2;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int pid1 = cthread_create(&thread1, mat_mul, &args);
    int pid2 = cthread_create(&thread2, mat_mul1, &args);
    int pid3 = cthread_create(&thread3, mat_mul2, &args);
    cthread_join(&thread1, NULL);
    cthread_join(&thread2, NULL);
    cthread_join(&thread3, NULL);
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    float micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    micros = micros / 1000000;
    printf("Time elapsed: %f seconds\n", micros);
    printf("Done\n");
    return 0;
}