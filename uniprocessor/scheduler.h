#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "../list.h"
#include "thread.h"
#include "queue.h"


#define MAX_READY_LIST_PRIORITY       5
#define MAX_READY_LIST_PRIORITY_ITEMS 20
// MAX_READY_LIST_PRIORITY*MAX_READY_LIST_PRIORITY_ITEMS + waiting threads + finished threads
#define MAX_THREADS                   1000


typedef struct {
    MFQueue_t   *ready_list;
    list_node_t *waiting_list;
    list_node_t *finished_list;

    tcb_t       *current_tcb;
    tcb_t       *all_tcb[MAX_THREADS];

    // SpinLock schedulerSpinLock;
} scheduler_t;

// shared variables
extern scheduler_t *scheduler;

extern void scheduler_start(void);
extern scheduler_t *scheduler_create(void);
extern void scheduler_free(scheduler_t *sched);
extern void schedule();

#endif
