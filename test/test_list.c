#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../list.h"

void
print_list(list_node_t *list)
{
    printf("list: [");
    list_node_t *l = list;
    while (l != NULL) {
        printf("\"%s\", ", l->data);
        l = l->next;
    }
    printf("]\n");
}

int main()
{
    printf("Start testing list...\n\n");

    printf("Create new list with one item:\n");
    list_node_t *list = list_create("item1");
    print_list(list);

    printf("\n========================\n\n");

    printf("Push a few items to the created list:\n");
    list_push(&list, "item2");
    list_push(&list, "item3");
    list_push(&list, "item4");
    print_list(list);

    printf("\n========================\n\n");

    printf("Popping all items:\n");
    list_node_t *popped;
    while ((popped = list_pop(&list))) {
        printf("Popped: \"%s\" | ", popped->data);
        print_list(list);
        list_free(popped);
    }

    printf("\n========================\n\n");

    printf("Push into NULL list:\n");
    list_push(&list, "new_item");
    print_list(list);

    list_free(list);
    return 0;
}
