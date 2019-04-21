#ifndef _TCB_H_
#define _TCB_H_

// TODO: remove it with ucontext.h
#define _XOPEN_SOURCE
#include <ucontext.h>


#define THREAD_STATE_INIT     0
#define THREAD_STATE_READY    1
#define THREAD_STATE_RUNNING  2
#define THREAD_STATE_WAITING  3
#define THREAD_STATE_FINISHED 4

#define INITIAL_STACK_SIZE 1024*64


#define thread_type unsigned long int
typedef thread_type thread_t;

typedef struct {
    // TODO: not used yet
    // Stack
    int stack_size;
    stack_t *stack;
    // Registers
    // regs_t *regs;
    // regs->sp = 123;
    int sp;
    int pc;

    // TODO: switch to asm implementation
    ucontext_t *context;

    // Metadata
    thread_t id;
    short int priority;
    short int state;
    void *result;
    thread_t join_id;

} tcb_t;


extern tcb_t *tcb_create(void);
extern tcb_t *get_tcb(thread_t *thread);
extern void tcb_free(tcb_t *tcb);
extern ucontext_t *context_create();
extern void context_free(ucontext_t *context);

#endif
