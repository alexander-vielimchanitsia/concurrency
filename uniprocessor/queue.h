#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "tcb.h"


typedef struct {
    unsigned int max_items;
    unsigned int front;
    unsigned int next_item;
    tcb_t **items;
} queue_t;


typedef struct {
    short int max_priority;
    queue_t **queues;
} MFQueue_t;


queue_t *queue_init(int max_items);
short int queue_push(queue_t *queue, tcb_t *item);
short int queue_pop(queue_t *queue, tcb_t **item);
void queue_free(queue_t *queue);

extern MFQueue_t *MFQueue_init(short int max_priority, int max_items);
extern short int MFQueue_push(MFQueue_t *q, tcb_t *item);
extern short int MFQueue_pop(MFQueue_t *q, tcb_t **item);
extern void MFQueue_free(MFQueue_t *queue);

#endif
