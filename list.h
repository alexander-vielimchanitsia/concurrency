#ifndef _LIST_H_
#define _LIST_H_

typedef struct ListNode {
    void *data;
    struct ListNode *next;
} list_node_t;

extern list_node_t *list_create(void *data);
extern void list_push(list_node_t **list, void *data);
extern list_node_t *list_pop(list_node_t **list);
extern void list_free(list_node_t *list);

#endif
