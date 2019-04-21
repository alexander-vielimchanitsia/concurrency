#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheduler.h"
#include "thread.h"


void *dummy_func(int *val)
{
    char *data = malloc(5);
    strcpy(data, "test");
    printf("inside dummy_func: %d\n", *val);
    return data;
}


void test_1()
{
    printf("create 1 thread\n");
    thread_t *thread = malloc(sizeof(thread_t));
    int arg = 1234;
    thread_create(thread, (void *)&dummy_func, (void *)&arg);
    printf("wait for thread to finish\n");
    thread_join(thread);
    printf("thread is finished\n");
}


int main()
{
    scheduler_start();
    test_1();
    return 0;
}
