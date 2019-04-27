#include "timer.h"

#include <signal.h>
#include <sys/time.h>


void
set_timer(int time)
{
    struct itimerval timer;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = time;
    setitimer(ITIMER_REAL, &timer, NULL);
}

void
cancel_timer()
{
    set_timer(0);
}
