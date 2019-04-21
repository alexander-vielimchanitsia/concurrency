/*
 Shared state:
  - code
  - global variables
  - heap

 Per-Tread State:
  - TCB
  - stack

 Thread Control Block (TCB):
  - Stack Information
  - Saved Registers
  - Thread Metadata
 */

#ifndef _THREAD_H_
#define _THREAD_H_


#include "tcb.h"

#define THREAD_START_PRIORITY 1

extern void thread_create(thread_t *thread, void *(*func) (void *), void *arg);
extern void thread_yield(void);
extern void *thread_join(thread_t *thread);
extern void thread_exit(void *ret);

#endif
