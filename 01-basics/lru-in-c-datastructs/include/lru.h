#ifndef LRU_H
#define LRU_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct LruCache LruCache;

/* Capacity zero is valid. NULL means allocation failure or excessive size. */
LruCache *lru_create(size_t capacity);
void lru_destroy(LruCache *cache);
/* Returns whether key was present; updates recency or inserts and evicts.
 * No allocation after creation. Expected O(1), worst-case O(capacity). */
bool lru_access(LruCache *cache, int64_t key);

#endif
