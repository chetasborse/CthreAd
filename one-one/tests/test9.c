#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>

void mat_muls(int ***a1, int ***b1, int ***c1, int r1, int c2, int r2) {
    int i, j, k, sum = 0;
    int **a, **b, **c;
    a = *a1;
    b = *b1;
    c = *c1;
    for(i = 0; i < r1; i++) {
        for(j = 0; j < c2; j++) {
            sum = 0;
            for(k = 0; k < r2; k++)
                sum += a[i][k] * b[k][j];    
            c[i][j] = sum;
        }
    }
}

int main() {
    int **a, **b, **c, r1, c1, r2, c2, i, j, k, sum = 0;
    
    scanf("%d%d", &r1, &c1);
    a = (int **)malloc(sizeof(int *) * r1);
    for(i = 0; i < r1; i++) {
        a[i] = (int *)malloc(sizeof(int) * c1);
        for(j = 0; j < c1; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    scanf("%d%d", &r2, &c2);
    if(c1 != r2) {
        printf("Columns of first matrix should be equal to the rows of the second matrix\n");
        return 1;
    }
    else {
        c = (int **)malloc(sizeof(int *) * r1);
        for(i = 0; i < r1; i++) {
            c[i] = (int *)malloc(sizeof(int) * c2); 
        }
    }
    b = (int **)malloc(sizeof(int *) * r2);
    for(i = 0; i < r2; i++) {
        b[i] = (int *)malloc(sizeof(int) * c2);
        for(j = 0; j < c2; j++) {
            scanf("%d", &b[i][j]);
        }
    }
    struct timeval start, end;
    gettimeofday(&start, NULL);
    mat_muls(&a, &b, &c, r1, c2, r2);
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    float micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    micros = micros / 1000000;
    printf("Time elapsed: %f seconds\n", micros);
    printf("Done\n");
    return 0;    
}