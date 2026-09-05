#include "lru.h"
#include "hash_map.h"
#include "list.h"

#include <stdlib.h>

/* Private payload owned by this component, not a list or map representation. */
typedef struct {
    PageId key;
    Page value;
} CacheEntry;

struct LruCache {
    List *recency;
    HashMap *index;
    size_t capacity, size;
};

static void evict_oldest(LruCache *cache) {
    ListNode *node = list_back(cache->recency);
    CacheEntry *entry = list_value(node);
    hash_map_remove(cache->index, entry->key);
    list_remove(cache->recency, node);
    free(entry);
    --cache->size;
}

static LruResult insert_page(LruCache *cache, PageId key,
                             SlowGetPage slow_get_page, Page *value) {
    CacheEntry *entry = malloc(sizeof(*entry));
    if (!entry)
        return LRU_NO_MEMORY;
    entry->key = key;
    ListNode *node = list_push_front(cache->recency, entry);
    if (!node) {
        free(entry);
        return LRU_NO_MEMORY;
    }
    HashMapResult result = hash_map_insert(cache->index, key, node);
    if (result != HASH_MAP_INSERTED) {
        list_remove(cache->recency, node);
        free(entry);
        return result == HASH_MAP_NO_MEMORY ? LRU_NO_MEMORY : LRU_INDEX_ERROR;
    }
    /* All fallible allocations precede loading and eviction. */
    entry->value = slow_get_page(key);
    if (cache->size == cache->capacity)
        evict_oldest(cache);
    ++cache->size;
    *value = entry->value;
    return LRU_MISS;
}

LruCache *lru_create(size_t capacity) {
    LruCache *cache = calloc(1, sizeof(*cache));
    if (!cache)
        return NULL;
    cache->recency = list_create();
    cache->index = hash_map_create(capacity ? capacity : 1);
    if (!cache->recency || !cache->index) {
        list_destroy(cache->recency);
        hash_map_destroy(cache->index);
        free(cache);
        return NULL;
    }
    cache->capacity = capacity;
    return cache;
}

void lru_destroy(LruCache *cache) {
    if (!cache)
        return;
    while (!list_empty(cache->recency))
        evict_oldest(cache);
    list_destroy(cache->recency);
    hash_map_destroy(cache->index);
    free(cache);
}

LruResult lookup_update(LruCache *cache, PageId key,
                        SlowGetPage slow_get_page, Page *value) {
    if (!cache || !value || !slow_get_page)
        return LRU_INVALID_ARGUMENT;
    if (!cache->capacity) {
        *value = slow_get_page(key);
        return LRU_MISS;
    }
    HashMapValue found;
    if (hash_map_find(cache->index, key, &found)) {
        ListNode *node = found;
        list_move_front(cache->recency, node);
        const CacheEntry *entry = list_value(node);
        *value = entry->value;
        return LRU_HIT;
    }
    return insert_page(cache, key, slow_get_page, value);
}
