#include "list.h"

#include <stdio.h>
#include <stdlib.h>


list_node_t *
list_create(void *data)
{
    list_node_t *list = malloc(sizeof(list_node_t));
    list->data = data;
    list->next = NULL;
    return list;
}


void
list_push(list_node_t **list, void *data)
{
    // if list is empty - so nothing to append to the next node
    if (*list == NULL) {
        *list = list_create(data);
        return;
    }

    list_node_t *tail = *list;
    while (tail->next != NULL)
        tail = tail->next;
    tail->next = list_create(data);
}


/*
 Pop the latest node out of the list.
 It'll set `list` to `NULL` when there's only one node in the list.
 */
list_node_t *
list_pop(list_node_t **list)
{
    if (*list == NULL)
        return NULL;

    list_node_t *node = *list;
    list_node_t *prev = NULL;
    while (node->next != NULL) {
        prev = node;
        node = node->next;
    }
    if (prev)
        prev->next = NULL;
    else
        *list = NULL;
    return node;
}


/*
 Remove a node by its index.
 */
list_node_t *
list_remove(list_node_t **list, int index)
{
    if (*list == NULL)
        return NULL;

    if (index < 0) {
        printf("list_remove: incorrect index - %d\n", index);
        return NULL;
    }

    list_node_t *node = NULL;
    list_node_t *current_node = *list;
    list_node_t *prev_node = NULL;

    for (int i = 0; current_node != NULL; i++) {
        if (index == i) {
            if (prev_node != NULL) {
                prev_node->next = current_node->next;
            }
            node = current_node;
            node->next = NULL;
            break;
        }
        prev_node = current_node;
        current_node = current_node->next;
    }

    return node;
}


void
list_free(list_node_t *list)
{
    list_node_t *next;
    while (list != NULL) {
        next = list->next;
        free(list);
        list = next;
    }
}
