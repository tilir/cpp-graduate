#ifndef INT_MAP_H
#define INT_MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Intrusive int64 -> void* map with fixed bucket count and separate chaining.
 * Caller owns entries and values and must keep their addresses stable.
 * Initialize entries to zero. Do not modify keys while linked. */
typedef struct IntMapEntry {
    int64_t key;
    void *value;
    struct IntMapEntry *next;
    struct IntMapEntry **prev_next;
} IntMapEntry;

typedef struct {
    IntMapEntry **buckets;
    size_t bucket_count;
} IntMap;

/* Initialize once before use. Failure leaves an empty, destroyable map. */
bool int_map_init(IntMap *map, size_t bucket_count);
void int_map_destroy(IntMap *map);
IntMapEntry *int_map_find(const IntMap *map, int64_t key);
/* Returns false for an already linked entry or duplicate key. */
bool int_map_insert(IntMap *map, IntMapEntry *entry);
/* Entry must belong to this map. Removal is O(1), even with collisions. */
void int_map_remove(IntMap *map, IntMapEntry *entry);

#endif
