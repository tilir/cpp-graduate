#include "int_map.h"
#include "list.h"
#include "lru.h"

#include <assert.h>
#include <stdint.h>

int main(void) {
    ListLink head, a, b;
    list_init(&head);
    list_init(&a);
    list_init(&b);
    assert(list_empty(&head));
    list_push_front(&head, &a);
    list_push_front(&head, &b);
    assert(head.next == &b && head.prev == &a);
    list_remove(&a);
    assert(head.prev == &b && b.next == &head);
    list_remove(&b);
    assert(list_empty(&head) && head.prev == &head);
    list_push_front(&head, &a);
    list_remove(&a);

    IntMap map;
    /* One bucket deliberately forces every entry into the same chain. */
    assert(int_map_init(&map, 1));
    IntMapEntry entries[3] = {{.key = INT64_MIN, .value = &a},
                              {.key = 0, .value = &b},
                              {.key = INT64_MAX, .value = &head}};
    for (int i = 0; i < 3; ++i)
        assert(int_map_insert(&map, &entries[i]));
    IntMapEntry duplicate = {.key = 0};
    assert(!int_map_insert(&map, &duplicate));
    assert(int_map_find(&map, 0)->value == &b);
    int_map_remove(&map, &entries[1]); /* Middle. */
    assert(!int_map_find(&map, 0));
    int_map_remove(&map, &entries[0]); /* Tail. */
    int_map_remove(&map, &entries[2]); /* Head and last. */
    for (int i = 0; i < 3; ++i)
        assert(int_map_insert(&map, &entries[i]));
    int_map_destroy(&map);
    for (int i = 0; i < 3; ++i)
        assert(!entries[i].prev_next && !entries[i].next);
    assert(!int_map_init(&map, SIZE_MAX));
    int_map_destroy(&map);

    LruCache *cache = lru_create(2);
    assert(cache);
    assert(!lru_access(cache, 1));
    assert(!lru_access(cache, 2));
    assert(lru_access(cache, 1));
    assert(!lru_access(cache, 3));
    assert(lru_access(cache, 1));
    assert(!lru_access(cache, 2));
    lru_destroy(cache);
    cache = lru_create(0);
    assert(cache && !lru_access(cache, 1) && !lru_access(cache, 1));
    lru_destroy(cache);
    assert(!lru_create(SIZE_MAX));
    lru_destroy(NULL);
    return 0;
}
