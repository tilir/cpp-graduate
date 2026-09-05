#include "hash_map.h"
#include "list.h"
#include "lru.h"

#include <assert.h>
#include <stdint.h>

static size_t loads;

static Page slow_get_page(PageId key) {
    ++loads;
    return key;
}

/* A second source proves that the cache returns payload, not the lookup key. */
static Page constant_page(PageId key) {
    (void)key;
    return 42;
}

static void test_cache(void) {
    Page page = -7;
    LruCache *cache = lru_create(2);
    assert(cache);
    assert(lookup_update(cache, 1, &slow_get_page, &page) == LRU_MISS && page == 1);
    assert(lookup_update(cache, 2, &slow_get_page, &page) == LRU_MISS && page == 2);
    assert(lookup_update(cache, 1, &slow_get_page, &page) == LRU_HIT && page == 1);
    assert(loads == 2);
    assert(lookup_update(cache, 3, &slow_get_page, &page) == LRU_MISS && page == 3);
    assert(lookup_update(cache, 1, &slow_get_page, &page) == LRU_HIT && page == 1);
    assert(lookup_update(cache, 2, &slow_get_page, &page) == LRU_MISS && page == 2);
    assert(loads == 4);
    assert(lookup_update(cache, 2, &slow_get_page, NULL) == LRU_INVALID_ARGUMENT);
    lru_destroy(cache);

    loads = 0;
    cache = lru_create(0);
    assert(cache);
    assert(lookup_update(cache, 0, &slow_get_page, &page) == LRU_MISS && page == 0);
    assert(lookup_update(cache, 0, &slow_get_page, &page) == LRU_MISS && page == 0);
    assert(loads == 2);
    lru_destroy(cache);

    cache = lru_create(1);
    assert(cache);
    assert(lookup_update(cache, 9, &constant_page, &page) == LRU_MISS && page == 42);
    assert(lookup_update(cache, 9, &slow_get_page, &page) == LRU_HIT && page == 42);
    assert(lookup_update(cache, 10, NULL, &page) == LRU_INVALID_ARGUMENT);
    assert(page == 42);
    assert(lookup_update(cache, 9, &slow_get_page, &page) == LRU_HIT);
    assert(lookup_update(cache, 10, &slow_get_page, &page) == LRU_MISS && page == 10);
    assert(lookup_update(cache, 9, &slow_get_page, &page) == LRU_MISS && page == 9);
    lru_destroy(cache);
    assert(!lru_create(SIZE_MAX));
    assert(lookup_update(NULL, 1, &slow_get_page, &page) == LRU_INVALID_ARGUMENT && page == 9);
    lru_destroy(NULL);
}

static void test_list(void) {
    int a = 1, b = 2, c = 3;
    List *list = list_create();
    assert(list && list_empty(list));
    assert(!list_front(list) && !list_back(list));
    ListNode *first = list_push_front(list, &a);
    ListNode *second = list_push_front(list, &b);
    ListNode *third = list_push_front(list, &c);
    assert(first && second && third);
    assert(list_value(list_front(list)) == &c);
    assert(list_value(list_back(list)) == &a);
    list_move_front(list, second); /* Middle to front. */
    assert(list_front(list) == second && list_back(list) == first);
    list_move_front(list, first); /* Tail to front. */
    list_move_front(list, first); /* Already front. */
    assert(list_front(list) == first && list_back(list) == third);
    list_remove(list, second);
    list_remove(list, third);
    assert(list_front(list) == first && list_back(list) == first);
    list_remove(list, first);
    assert(list_empty(list));
    assert(list_push_front(list, NULL));
    assert(list_value(list_front(list)) == NULL);
    list_destroy(list); /* Also frees remaining nodes, never payloads. */
    list_destroy(NULL);
}

static void test_map(void) {
    int values[3] = {1, 2, 3};
    HashMapKey keys[3] = {INT64_MIN, 0, INT64_MAX};
    HashMap *map = hash_map_create(1); /* Force every key into one chain. */
    assert(map);
    for (int i = 0; i < 3; ++i)
        assert(hash_map_insert(map, keys[i], &values[i]) == HASH_MAP_INSERTED);
    assert(hash_map_insert(map, 0, NULL) == HASH_MAP_DUPLICATE);
    HashMapValue value;
    assert(hash_map_find(map, 0, &value) && value == &values[1]);
    assert(hash_map_remove(map, 0)); /* Middle. */
    assert(!hash_map_find(map, 0, &value) && value == &values[1]);
    assert(!hash_map_remove(map, 0));
    assert(hash_map_remove(map, INT64_MIN)); /* Tail. */
    assert(hash_map_remove(map, INT64_MAX)); /* Last. */
    for (int i = 0; i < 3; ++i)
        assert(hash_map_insert(map, keys[i], &values[i]) == HASH_MAP_INSERTED);
    assert(hash_map_remove(map, INT64_MAX)); /* Head with remaining chain. */
    assert(hash_map_find(map, 0, &value) && value == &values[1]);
    assert(hash_map_insert(map, 7, NULL) == HASH_MAP_INSERTED);
    assert(hash_map_find(map, 7, &value) && value == NULL);
    hash_map_destroy(map);
    assert(!hash_map_create(0));
    assert(!hash_map_create(SIZE_MAX));
    hash_map_destroy(NULL);
}

int main(void) {
    test_list();
    test_map();
    test_cache();
    return 0;
}
