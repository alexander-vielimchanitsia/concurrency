#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../list.h"
#include "tcb.h"
#include "thread.h"
#include "scheduler.h"
#include "queue.h"


scheduler_t *scheduler = NULL;


/*
 We enter as old_thread, but we return as new_thread.
 Returns with new_thread's registers and stack.
 */
void
_thread_switch(tcb_t *old_thread, tcb_t *new_thread)
{
    // pushad;                 // Push general register values onto the old stack. 
    // old_thread->sp = %esp;  // Save the old thread's stack pointer;
    // %esp = new_thread->sp;  // Switch to the new stack.
    // popad;                  // Pop register values from the new stack.
    // return;

    // TODO: remove it (only for prototype)
    printf("_thread_switch - from: %lu | to: %lu\n", old_thread->id, new_thread->id);
    swapcontext(old_thread->context, new_thread->context);
    printf("after switch!\n");
}


void
scheduler_start()
{
    assert(scheduler == NULL);
    scheduler = scheduler_create();
    // create a tcb for current thread(the main thread that called this function)
    scheduler->current_tcb = tcb_create();
    // TODO: figure out why do we need to add 105 to the size! error in swapcontext otherwise
    scheduler->current_tcb->context = malloc(sizeof(*scheduler->current_tcb->context)+105);
    scheduler->current_tcb->state = THREAD_STATE_RUNNING;

    // TODO: start timer interrupt
}


scheduler_t *
scheduler_create()
{
    scheduler_t *sched = malloc(sizeof(scheduler_t));
    sched->ready_list = MFQueue_init(MAX_READY_LIST_PRIORITY, MAX_READY_LIST_PRIORITY_ITEMS);
    sched->waiting_list = NULL;
    sched->finished_list = NULL;
    sched->current_tcb = NULL;
    return sched;
}


void
scheduler_free(scheduler_t *sched)
{
    MFQueue_free(sched->ready_list);
    list_free(sched->waiting_list);
    list_free(sched->finished_list);
    free(sched);
}

void
schedule()
{
    tcb_t *next_thread, *current_tcb;
    if (MFQueue_pop(scheduler->ready_list, &next_thread)) {

        // Move running thread onto the ready list
        if (scheduler->current_tcb->state == THREAD_STATE_RUNNING) {
            scheduler->current_tcb->state = THREAD_STATE_READY;
            MFQueue_push(scheduler->ready_list, scheduler->current_tcb);
        }

        // Switch to the new thread.
        next_thread->state = THREAD_STATE_RUNNING;
        current_tcb = scheduler->current_tcb;
        scheduler->current_tcb = next_thread;
        _thread_switch(current_tcb, next_thread);
    } else {
        printf("schedule: nothing else to run, continue to run current thread.\n");
    }
}

