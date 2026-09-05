#include "lru.h"
#include "int_map.h"
#include "list.h"

#include <assert.h>
#include <stdlib.h>

typedef struct {
    ListLink recency;
    IntMapEntry mapping;
} CacheEntry;

struct LruCache {
    ListLink recency;
    IntMap index;
    CacheEntry *entries;
    size_t capacity, used;
};

LruCache *lru_create(size_t capacity) {
    if (capacity > SIZE_MAX / sizeof(CacheEntry))
        return NULL;
    LruCache *cache = calloc(1, sizeof(*cache));
    if (!cache)
        return NULL;
    list_init(&cache->recency);
    if (capacity) {
        cache->entries = calloc(capacity, sizeof(*cache->entries));
        if (!cache->entries || !int_map_init(&cache->index, capacity)) {
            lru_destroy(cache);
            return NULL;
        }
    }
    cache->capacity = capacity;
    return cache;
}

void lru_destroy(LruCache *cache) {
    if (!cache)
        return;
    int_map_destroy(&cache->index);
    free(cache->entries);
    free(cache);
}

bool lru_access(LruCache *cache, int64_t key) {
    if (!cache->capacity)
        return false;
    IntMapEntry *found = int_map_find(&cache->index, key);
    CacheEntry *entry;
    if (found) {
        entry = found->value;
        list_remove(&entry->recency);
    } else {
        if (cache->used < cache->capacity) {
            entry = &cache->entries[cache->used++];
            list_init(&entry->recency);
        } else {
            /* recency is the first member: conversion is defined by C. */
            entry = (CacheEntry *)cache->recency.prev;
            list_remove(&entry->recency);
            int_map_remove(&cache->index, &entry->mapping);
        }
        entry->mapping.key = key;
        entry->mapping.value = entry;
        bool inserted = int_map_insert(&cache->index, &entry->mapping);
        assert(inserted);
        (void)inserted;
    }
    list_push_front(&cache->recency, &entry->recency);
    return found != NULL;
}
