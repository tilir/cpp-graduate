#ifndef LRU_H
#define LRU_H

#include <stddef.h>
#include <stdint.h>

typedef int64_t PageId;
/* Value representation for this example; no separate page allocation. */
typedef int64_t Page;
typedef Page (*SlowGetPage)(PageId key);
typedef struct LruCache LruCache;

typedef enum {
    LRU_HIT,
    LRU_MISS,
    LRU_NO_MEMORY,
    LRU_INVALID_ARGUMENT,
    LRU_INDEX_ERROR
} LruResult;

/* NULL on excessive capacity or allocation failure. */
LruCache *lru_create(size_t capacity);
void lru_destroy(LruCache *cache);
/* Copy the page to *value. Errors leave *value and cache contents unchanged.
 * slow_get_page is required and called only on misses; it is not stored.
 * Changing loaders does not invalidate existing pages for the same keys.
 * The loader must return a page for every key and must not reenter this cache.
 * Capacity zero always calls the loader and stores nothing.
 * Not thread-safe. Expected O(1) excluding loader, worst O(capacity). */
LruResult lookup_update(LruCache *cache, PageId key,
                        SlowGetPage slow_get_page, Page *value);

#endif
