#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../thread.h"
#include "../queue.h"


int main()
{
    printf("Start testing queue...\n");
    MFQueue_t *queue = MFQueue_init(4, 10);
    tcb_t *item1, *item2, *item3, *itempop;
    printf("Empty queue pop: %d\n", MFQueue_pop(queue, &itempop));

    // FIXME: use `_create_tcb` instead
    item1 = malloc(sizeof(tcb_t));
    item1->id = 111;
    item1->priority = 1;
    item2 = malloc(sizeof(tcb_t));
    item2->id = 222;
    item2->priority = 2;
    item3 = malloc(sizeof(tcb_t));
    item3->id = 333;
    item3->priority = 3;

    printf("Push item: %d | id: %d\n", MFQueue_push(queue, item2), (int)item2->id);
    printf("Push item: %d | id: %d\n", MFQueue_push(queue, item3), (int)item3->id);
    printf("Push item: %d | id: %d\n", MFQueue_push(queue, item1), (int)item1->id);

    printf("Pop item: %d | id: %d\n", MFQueue_pop(queue, &itempop), (int)itempop->id);
    assert(itempop->id == item1->id);
    printf("Pop item: %d | id: %d\n", MFQueue_pop(queue, &itempop), (int)itempop->id);
    assert(itempop->id == item2->id);
    printf("Pop item: %d | id: %d\n", MFQueue_pop(queue, &itempop), (int)itempop->id);
    assert(itempop->id == item3->id);

    MFQueue_free(queue);
    return 0;
}
