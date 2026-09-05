#include "hash_map.h"

#include <stdlib.h>

typedef struct HashMapEntry {
    HashMapKey key;
    HashMapValue value;
    struct HashMapEntry *next;
} HashMapEntry;

struct HashMap {
    HashMapEntry **buckets;
    size_t bucket_count;
};

static size_t bucket_for(const HashMap *map, HashMapKey key) {
    uint64_t hash = (uint64_t)key;
    hash ^= hash >> 30;
    hash *= UINT64_C(0xbf58476d1ce4e5b9);
    hash ^= hash >> 27;
    hash *= UINT64_C(0x94d049bb133111eb);
    hash ^= hash >> 31;
    return (size_t)(hash % map->bucket_count);
}

HashMap *hash_map_create(size_t bucket_count) {
    if (!bucket_count || bucket_count > SIZE_MAX / sizeof(HashMapEntry *))
        return NULL;
    HashMap *map = malloc(sizeof(*map));
    if (!map)
        return NULL;
    map->buckets = calloc(bucket_count, sizeof(*map->buckets));
    if (!map->buckets) {
        free(map);
        return NULL;
    }
    map->bucket_count = bucket_count;
    return map;
}

void hash_map_destroy(HashMap *map) {
    if (!map)
        return;
    for (size_t i = 0; i < map->bucket_count; ++i) {
        HashMapEntry *entry = map->buckets[i];
        while (entry) {
            HashMapEntry *next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(map->buckets);
    free(map);
}

bool hash_map_find(const HashMap *map, HashMapKey key, HashMapValue *value) {
    for (HashMapEntry *entry = map->buckets[bucket_for(map, key)];
         entry; entry = entry->next) {
        if (entry->key == key) {
            *value = entry->value;
            return true;
        }
    }
    return false;
}

HashMapResult hash_map_insert(HashMap *map, HashMapKey key, HashMapValue value) {
    HashMapValue existing;
    if (hash_map_find(map, key, &existing))
        return HASH_MAP_DUPLICATE;
    HashMapEntry *entry = malloc(sizeof(*entry));
    if (!entry)
        return HASH_MAP_NO_MEMORY;
    size_t bucket = bucket_for(map, key);
    *entry = (HashMapEntry){key, value, map->buckets[bucket]};
    map->buckets[bucket] = entry;
    return HASH_MAP_INSERTED;
}

bool hash_map_remove(HashMap *map, HashMapKey key) {
    HashMapEntry **link = &map->buckets[bucket_for(map, key)];
    while (*link && (*link)->key != key)
        link = &(*link)->next;
    if (!*link)
        return false;
    HashMapEntry *entry = *link;
    *link = entry->next;
    free(entry);
    return true;
}
