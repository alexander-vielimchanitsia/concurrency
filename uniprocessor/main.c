#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheduler.h"
#include "thread.h"


// ======================================

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

// ======================================

void
loop(int *thread_id)
{
    int i = 0;
    while (1)
        if (i++ % 10000000 == 0)
            printf("thread(%d) tick\n", *thread_id);
}

void
test_infinity_loop()
{
    printf("creating a thread for another loop\n");
    thread_t *thread = malloc(sizeof(thread_t));
    int thread_id = 1;
    thread_create(thread, (void *)&loop, (void *)&thread_id);

    printf("starting loop in the main thread\n");
    int main_thread_id = 0;
    loop(&main_thread_id);
}

// ======================================

int main()
{
    scheduler_start();
    // test_1();
    test_infinity_loop();
    return 0;
}
