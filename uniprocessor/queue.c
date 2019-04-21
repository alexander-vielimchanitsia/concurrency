#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


queue_t *
queue_init(int max_items)
{
    queue_t *queue = malloc(sizeof(queue_t));
    queue->max_items = max_items;
    queue->front = 0;
    queue->next_item = 0;
    queue->items = malloc(sizeof(**queue->items) * max_items);
    return queue;
}

short int
queue_push(queue_t *queue, tcb_t *item)
{
    short int success = 0;
    if ((queue->next_item - queue->front) < queue->max_items) {
        queue->items[queue->next_item++ % queue->max_items] = item;
        success = 1;
    }
    return success;
}

short int
queue_pop(queue_t *queue, tcb_t **item)
{
    short int success = 0;
    if (queue->front < queue->next_item) {
        *item = queue->items[queue->front++ % queue->max_items];
        success = 1;
    }
    return success;
}

void
queue_free(queue_t *queue)
{
    tcb_t *item;
    while (queue_pop(queue, &item))
        free(item);
    free(queue);
}

/* Multi-level Feedback Queue (MFQ) */
// ==================================

MFQueue_t *
MFQueue_init(short int max_priority, int max_items)
{
    assert(max_priority > 0);
    MFQueue_t *MFQueue = malloc(sizeof(MFQueue_t));
    MFQueue->max_priority = max_priority;
    // create queue for each priority
    int priority = 1;
    MFQueue->queues = malloc(sizeof(**MFQueue->queues) * max_priority + priority);
    for (; priority <= max_priority; priority++)
        MFQueue->queues[priority] = queue_init(max_items);
    return MFQueue;
}


short int
MFQueue_push(MFQueue_t *q, tcb_t *item)
{
    if (item->priority > q->max_priority) {
        printf("MFQueue_push: thread(%d) has too high priority: %d, max allowed: %d\n",
               (int)item->id, item->priority, q->max_priority);
        return 0;
    }
    return queue_push(q->queues[item->priority], item);
}


// TODO: fix strvation of the lowest priotity's items, when there're always some items with a higher priotity
short int
MFQueue_pop(MFQueue_t *q, tcb_t **item)
{
    short int success = 0;
    for (short int priority = 1; priority <= q->max_priority; priority++) {
        success = queue_pop(q->queues[priority], item);
        if (success)
            break;
    }
    return success;
}


void
MFQueue_free(MFQueue_t *queue)
{
    for (short int priotity = 1; priotity <= queue->max_priority; priotity++)
        queue_free(queue->queues[priotity]);
    free(queue);
}
