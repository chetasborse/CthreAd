#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

#define PERIOD 100000
static sigset_t block;
void timer_handler();

static void init() __attribute__((constructor));
void init()
{
    sigemptyset(&block);
    sigaddset(&block, SIGVTALRM);

    struct sigaction act = {0};
    struct timeval interval;
    struct itimerval period;

    act.sa_handler = timer_handler;
    assert(sigaction(SIGVTALRM, &act, NULL) == 0);

    interval.tv_sec = 0;
    interval.tv_usec = PERIOD;
    period.it_interval = interval;
    period.it_value = interval;
    setitimer(ITIMER_VIRTUAL, &period, NULL);
}

void timer_handler(int sig)
{
    write(1, "Hi\n", 3);
}
int main()
{
    while (1)
        ;
    return 0;
}