#include "list.h"

void list_init(ListLink *link) {
    link->prev = link->next = link;
}

bool list_empty(const ListLink *head) {
    return head->next == head;
}

void list_push_front(ListLink *head, ListLink *link) {
    link->prev = head;
    link->next = head->next;
    head->next->prev = link;
    head->next = link;
}

void list_remove(ListLink *link) {
    link->prev->next = link->next;
    link->next->prev = link->prev;
    list_init(link);
}
