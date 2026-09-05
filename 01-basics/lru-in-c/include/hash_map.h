#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct HashMap HashMap;
typedef int64_t HashMapKey;
typedef void *HashMapValue;
typedef enum {
    HASH_MAP_INSERTED,
    HASH_MAP_DUPLICATE,
    HASH_MAP_NO_MEMORY
} HashMapResult;

/* Map owns internal entries, caller owns values. bucket_count must be positive.
 * NULL on allocation failure or excessive size. */
HashMap *hash_map_create(size_t bucket_count);
void hash_map_destroy(HashMap *map);
/* Supports NULL values: success is distinguished by the boolean result.
 * On absence, *value is unchanged. */
bool hash_map_find(const HashMap *map, HashMapKey key, HashMapValue *value);
/* Duplicate keys do not replace existing values. */
HashMapResult hash_map_insert(HashMap *map, HashMapKey key, HashMapValue value);
bool hash_map_remove(HashMap *map, HashMapKey key);
/* Lookup, insert and remove: expected O(1) at bounded load, worst O(size). */

#endif
