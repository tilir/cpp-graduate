#include "list.h"

#include <stdlib.h>

struct ListNode {
    struct ListNode *prev, *next;
    ListValue value;
};

struct List {
    ListNode head;
};

List *list_create(void) {
    List *list = malloc(sizeof(*list));
    if (list) {
        list->head.next = list->head.prev = &list->head;
        list->head.value = NULL;
    }
    return list;
}

bool list_empty(const List *list) {
    return list->head.next == &list->head;
}

ListNode *list_front(const List *list) {
    return list_empty(list) ? NULL : list->head.next;
}

ListNode *list_back(const List *list) {
    return list_empty(list) ? NULL : list->head.prev;
}

ListValue list_value(const ListNode *node) {
    return node->value;
}

static void prepend(List *list, ListNode *node) {
    node->prev = &list->head;
    node->next = list->head.next;
    node->next->prev = node;
    list->head.next = node;
}

static void detach(ListNode *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

ListNode *list_push_front(List *list, ListValue value) {
    ListNode *node = malloc(sizeof(*node));
    if (!node)
        return NULL;
    node->value = value;
    prepend(list, node);
    return node;
}

void list_move_front(List *list, ListNode *node) {
    detach(node);
    prepend(list, node);
}

void list_remove(List *list, ListNode *node) {
    (void)list;
    detach(node);
    free(node);
}

void list_destroy(List *list) {
    if (!list)
        return;
    while (!list_empty(list))
        list_remove(list, list_front(list));
    free(list);
}
