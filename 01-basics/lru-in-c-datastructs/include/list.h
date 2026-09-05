#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

/* Intrusive circular list: embed a link in any payload. The sentinel owns no
 * payload. Removing a link requires membership; insertion requires detachment.
 * Lists and linked objects must not be copied or moved in memory. */
typedef struct ListLink {
    struct ListLink *prev, *next;
} ListLink;

void list_init(ListLink *link);
bool list_empty(const ListLink *head);
void list_push_front(ListLink *head, ListLink *link);
void list_remove(ListLink *link);

#endif
