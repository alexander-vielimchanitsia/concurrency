#include <stdio.h>
#include <stdlib.h>

#include "../list.h"
#include "scheduler.h"
#include "tcb.h"
#include "queue.h"

#include "thread.h"


void
_execute_thread_func(void *(*func) (void *), void *arg)
{
    void *ret = (*func)(arg);
    // If func does not call `exit`, call it here.
    thread_exit(ret);
    printf("return ...\n");
}


/*
 `thread_create` must put a dummy frame at the top of its stack:
 the return PC and space for `pushad` to have stored a copy of the registers.
 This way, when someone switches to a newly created thread,
 the last two lines of `thread_switch` work correctly.
 */
// void
// thread_dummySwitchFrame(tcb_t *new_thread)
// {
//     *(new_thread->sp) = &_execute_thread_func;
//     new_thread->sp--;
//     new_thread->sp -= size_of_popad;  // FIXME
// }


/*
 Create a new thread, storing information aboit it in `thread`.
 Concurrently with the calling thread, `thread` executes the `func` with the argument `arg`.
 */
void
thread_create(thread_t *thread, void *(*func) (void *), void *arg)
{
    // create TCB
    tcb_t *tcb = tcb_create();
    *thread = tcb->id;

    tcb->context = context_create();
    makecontext(tcb->context, &_execute_thread_func, 2, func, arg);

    // tcb->stack_size = INITIAL_STACK_SIZE;
    // tcb->stack = stack_create(INITIAL_STACK_SIZE);

    // Initialize registers so that when thread is resumed, it will start running at `_execute_thread_func`.
    // tcb->pc = &_execute_thread_func;
    // The stack starts at the top of the allocate region and grows down.
    // tcb->sp = tcb->stack + INITIAL_STACK_SIZE;

    // Create a stack frame by pushing _execute_thread_func's arguments and start address.
    // *(tcb->sp) = arg;
    // tcb->sp--;
    // *(tcb->sp) = func;
    // tcb->sp--;

    // Create another stack frame so that `thread_switch` works correctly
    // thread_dummySwitchFrame(tcb);

    tcb->state = THREAD_STATE_READY;
    if (!MFQueue_push(scheduler->ready_list, tcb)) {
        printf("thread_create: failed to push tcb into the ready_list. tcb->id=%lu\n", tcb->id);
        tcb_free(tcb);
    }
}

// TODO: make it work with register accesses
// void
// thread_create(thread_t *thread, void *(*func) (void *), void *arg)
// {
//     // create TCB
//     tcb_t *tcb = _create_tcb();
//     *thread = tcb->id;

//     tcb->stack_size = INITIAL_STACK_SIZE;
//     tcb->stack = _create_stack(INITIAL_STACK_SIZE);

//     // Initialize registers so that when thread is resumed, it will start running at `_execute_thread_func`.
//     tcb->pc = &_execute_thread_func;
//     // The stack starts at the top of the allocate region and grows down.
//     tcb->sp = tcb->stack + INITIAL_STACK_SIZE;

//     // Create a stack frame by pushing _execute_thread_func's arguments and start address.
//     *(tcb->sp) = arg;
//     tcb->sp--;
//     *(tcb->sp) = func;
//     tcb->sp--;

//     // Create another stack frame so that `thread_switch` works correctly
//     thread_dummySwitchFrame(tcb);

//     tcb->state = THREAD_STATE_READY;
//     scheduler->ready_list.add(tcb);
// }


/*
 The calling thread voluntarily gives up the processor to let some other thread(s) run.
 The scheduler can resume running the calling thread whenever it chooses to do so.
 */
void
thread_yield()
{
    list_node_t *finished_node;

    // Prevent an interrupt from stopping us in the middle of a switch.
    // disable_interrupts();

    schedule();

    // if there's nothing else to run -> go back to running the original thread.

    // TODO: here's a bug maybe, looks like it can remove a finished thread when a joiner thread hasn't got a result.
    // Delete any threads on the finished list.
    while ((finished_node = list_pop(&scheduler->finished_list)) != NULL) {
        tcb_free(finished_node->data);
        list_free(finished_node);
    }

    // enable_interrupts();
}


/*
 Wait for `thread` to finish if it has not already done so;
 then reuturn the value passed to `thread_exit` by that `thread`.
 Note that `thread_join` may be called only once for each thread. TODO: <- make it
 */
void *
thread_join(thread_t *thread_id)
{
    tcb_t *thread = get_tcb(thread_id);
    // if need to wait -> switch to the next thread
    if (thread->state != THREAD_STATE_FINISHED) {

        // move current thread to the waiting list
        scheduler->current_tcb->state = THREAD_STATE_WAITING;
        scheduler->current_tcb->join_id = *thread_id;
        list_push(&scheduler->waiting_list, scheduler->current_tcb);

        schedule();
    }

    return thread->result;
}


/*
 Fininsh the current thread. Store the value ret in the current thread's TCB.
 If another thread already waiting in a call to `thread_join`, resume it.
 */
void
thread_exit(void *ret)
{
    scheduler->current_tcb->result = ret;
    scheduler->current_tcb->state = THREAD_STATE_FINISHED;
    list_push(&scheduler->finished_list, scheduler->current_tcb);

    printf("thread_exit: result - %s\n", ret);

    // wake up joiner, if any
    tcb_t *joiner = NULL;
    list_node_t *waiting_thread = NULL;
    for (waiting_thread = scheduler->waiting_list; waiting_thread; waiting_thread = waiting_thread->next) {
        if (waiting_thread->data && ((tcb_t *)waiting_thread->data)->join_id == scheduler->current_tcb->id) {
            joiner = waiting_thread->data;
            // TODO: remove it from the waiting list
            break;
        }
    }
    if (joiner) {
        joiner->state = THREAD_STATE_READY;
        joiner->join_id = -1;
        if (!MFQueue_push(scheduler->ready_list, joiner)) {
            printf("thread_exit: failed to move waiting thread to ready list - list is full, thread=%lu\n", joiner->id);
            exit(1);
        }
    }

    schedule();
}
