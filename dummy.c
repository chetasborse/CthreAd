#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

void sig_handler(int sig_no)
{
    alarm(2);
    printf("Hello!");
    return;
}

int main()
{
    signal(SIGALRM, sig_handler);
    alarm(1);
    for (int i = 0; i < 10; i++)
    {
        puts("hey");
        sleep(1);
    }
    return 0;
}