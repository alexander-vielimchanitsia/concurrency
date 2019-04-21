#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>


static void clock_tick(int signo)
{
    printf("clock tick!!\n");
}


void timer_init()
{
    signal(SIGALRM, clock_tick);

    struct itimerval tv;
    tv.it_interval.tv_sec = 0;
    tv.it_interval.tv_usec = 100000;  // when timer expires, reset to 100ms
    tv.it_value.tv_sec = 0;
    tv.it_value.tv_usec = 100000;
    setitimer(ITIMER_REAL, &tv, NULL);
}
