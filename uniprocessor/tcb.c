/*
 Thread Control Block
 */

// TODO: remove it with ucontext.h
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#include "scheduler.h"
#include "thread.h"
#include "tcb.h"


thread_t next_tcb_id = 0;


tcb_t *
tcb_create()
{
    tcb_t *tcb = malloc(sizeof(tcb_t));
    tcb->id = next_tcb_id++;
    tcb->priority = THREAD_START_PRIORITY;
    tcb->state = THREAD_STATE_INIT;
    tcb->join_id = -1;

    tcb->stack_size = 0;
    tcb->stack = NULL;

    scheduler->all_tcb[tcb->id] = tcb;
    return tcb;
}


tcb_t *
get_tcb(thread_t *thread)
{
    if (*thread < 0 || *thread > MAX_THREADS) {
        printf("get_tcb: wrong thread id, allowed: 0-%d\n", MAX_THREADS);
        exit(1);
    }
    return scheduler->all_tcb[(thread_type)*thread];
}

void
tcb_free(tcb_t *tcb)
{
    context_free(tcb->context);
    free(tcb);
}

ucontext_t *
context_create()
{
    ucontext_t *context = malloc(sizeof(ucontext_t));
    getcontext(context);

    // set stack
    context->uc_link = 0;
    context->uc_stack.ss_sp = malloc(INITIAL_STACK_SIZE);
    context->uc_stack.ss_size = INITIAL_STACK_SIZE;
    context->uc_stack.ss_flags = 0;        
    if (!context->uc_stack.ss_sp)
    {
         perror( "stack_create: failed allocate stack" );
         exit( 1 );
    }

    return context;
}

void
context_free(ucontext_t *context)
{
    if (context) {
        free(context->uc_stack.ss_sp);
        free(context);
    }
}
