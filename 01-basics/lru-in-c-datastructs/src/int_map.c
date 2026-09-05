#include "int_map.h"

#include <stdlib.h>

static size_t bucket_for(const IntMap *map, int64_t key) {
    uint64_t hash = (uint64_t)key;
    hash ^= hash >> 30;
    hash *= UINT64_C(0xbf58476d1ce4e5b9);
    hash ^= hash >> 27;
    hash *= UINT64_C(0x94d049bb133111eb);
    hash ^= hash >> 31;
    return (size_t)(hash % map->bucket_count);
}

bool int_map_init(IntMap *map, size_t bucket_count) {
    *map = (IntMap){0};
    if (!bucket_count || bucket_count > SIZE_MAX / sizeof(*map->buckets))
        return false;
    map->buckets = calloc(bucket_count, sizeof(*map->buckets));
    if (!map->buckets)
        return false;
    map->bucket_count = bucket_count;
    return true;
}

void int_map_remove(IntMap *map, IntMapEntry *entry) {
    (void)map;
    *entry->prev_next = entry->next;
    if (entry->next)
        entry->next->prev_next = entry->prev_next;
    entry->next = NULL;
    entry->prev_next = NULL;
}

void int_map_destroy(IntMap *map) {
    for (size_t i = 0; i < map->bucket_count; ++i)
        while (map->buckets[i])
            int_map_remove(map, map->buckets[i]);
    free(map->buckets);
    *map = (IntMap){0};
}

IntMapEntry *int_map_find(const IntMap *map, int64_t key) {
    if (!map->bucket_count)
        return NULL;
    for (IntMapEntry *entry = map->buckets[bucket_for(map, key)];
         entry; entry = entry->next)
        if (entry->key == key)
            return entry;
    return NULL;
}

bool int_map_insert(IntMap *map, IntMapEntry *entry) {
    if (!map->bucket_count || entry->prev_next || int_map_find(map, entry->key))
        return false;
    IntMapEntry **head = &map->buckets[bucket_for(map, entry->key)];
    entry->next = *head;
    entry->prev_next = head;
    if (*head)
        (*head)->prev_next = &entry->next;
    *head = entry;
    return true;
}
