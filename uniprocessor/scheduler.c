#include "scheduler.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../list.h"
#include "tcb.h"
#include "thread.h"
#include "timer.h"
#include "queue.h"


scheduler_t *scheduler = NULL;

static int _interrupts_enabled = 1;

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
    swapcontext(old_thread->context, new_thread->context);
}


void
_timer_interrupt_handler()
{
    // TODO: still should save any interrupts, but handle them when the interrupts are enabled
    if (!_interrupts_enabled)
        return;

    if (scheduler->current_tcb->state != THREAD_STATE_RUNNING) {
        printf("_timer_interrupt_handler: something went wrong - current thread is not running.\n");
        exit(-1);
    }

    // move to a lower priority when the thread has more work to do than the time quantum
    if (scheduler->current_tcb->priority != MAX_READY_LIST_PRIORITY)
        scheduler->current_tcb->priority++;

    // TODO: maybe better to update data in stack frame of the interrupt?
    schedule();
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
    if (!scheduler->current_tcb->context) {
        printf("Failed to allocate context for the main thread.\n");
        exit(1);
    }
    scheduler->current_tcb->state = THREAD_STATE_RUNNING;

    // timer interrupt
    signal(SIGALRM, _timer_interrupt_handler);
    set_timer(get_thread_time_quantum(scheduler->current_tcb));
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
        set_timer(get_thread_time_quantum(scheduler->current_tcb));
        _thread_switch(current_tcb, next_thread);
    } else {
        printf("schedule: nothing else to run, continue to run current thread.\n");
    }
}


void
disable_interrups()
{
    _interrupts_enabled = 0;
}


void
enable_interrupts()
{
    _interrupts_enabled = 1;
}

