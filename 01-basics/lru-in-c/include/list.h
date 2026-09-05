#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct List List;
typedef struct ListNode ListNode;
typedef void *ListValue;

/* List owns nodes, caller owns payloads. NULL on allocation failure. */
List *list_create(void);
void list_destroy(List *list);
bool list_empty(const List *list);
ListNode *list_push_front(List *list, ListValue value);
ListNode *list_front(const List *list);
ListNode *list_back(const List *list);
ListValue list_value(const ListNode *node);
/* node must belong to list. Moving preserves its address; removal invalidates it.
 * All operations except create/destroy are O(1). */
void list_move_front(List *list, ListNode *node);
void list_remove(List *list, ListNode *node);

#endif
