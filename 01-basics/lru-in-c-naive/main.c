#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
struct Node {
    int64_t value;
    Node *prev, *next; /* Most recently used at the head. */
    Node *hash_next;
};

typedef struct {
    Node *head, *tail;
    Node **buckets;
    size_t capacity;
} Cache;

static size_t bucket_for(int64_t value, size_t capacity) {
    /* Mix all bits, including those of negative keys, using unsigned math. */
    uint64_t hash = (uint64_t)value;
    hash ^= hash >> 30;
    hash *= UINT64_C(0xbf58476d1ce4e5b9);
    hash ^= hash >> 27;
    hash *= UINT64_C(0x94d049bb133111eb);
    hash ^= hash >> 31;
    return (size_t)(hash % capacity);
}

static void unlink_node(Cache *cache, Node *node) {
    if (node->prev)
        node->prev->next = node->next;
    else
        cache->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        cache->tail = node->prev;
}

static void prepend(Cache *cache, Node *node) {
    node->prev = NULL;
    node->next = cache->head;
    if (cache->head)
        cache->head->prev = node;
    else
        cache->tail = node;
    cache->head = node;
}

int main(void) {
    int64_t requested_capacity, count;
    if (scanf("%" SCNd64 " %" SCNd64, &requested_capacity, &count) != 2 ||
        requested_capacity < 0 || count < 0) {
        fprintf(stderr, "Expected nonnegative cache size and element count.\n");
        return EXIT_FAILURE;
    }

    /* No more than count distinct elements can be stored. */
    uint64_t limit = (uint64_t)(requested_capacity < count
                                  ? requested_capacity : count);
    if (limit > SIZE_MAX / sizeof(Node) ||
        limit > SIZE_MAX / sizeof(Node *)) {
        fprintf(stderr, "Cache is too large.\n");
        return EXIT_FAILURE;
    }

    Cache cache = {0};
    cache.capacity = (size_t)limit;
    Node *nodes = NULL;
    if (cache.capacity) {
        nodes = calloc(cache.capacity, sizeof(*nodes));
        cache.buckets = calloc(cache.capacity, sizeof(*cache.buckets));
        if (!nodes || !cache.buckets) {
            fprintf(stderr, "Cannot allocate cache.\n");
            free(nodes);
            free(cache.buckets);
            return EXIT_FAILURE;
        }
    }

    size_t used = 0;
    int64_t hits = 0;
    for (int64_t i = 0; i < count; ++i) {
        int64_t value;
        if (scanf("%" SCNd64, &value) != 1) {
            fprintf(stderr, "Expected element at position %" PRId64 ".\n", i + 1);
            free(nodes);
            free(cache.buckets);
            return EXIT_FAILURE;
        }
        if (!cache.capacity)
            continue;

        size_t bucket = bucket_for(value, cache.capacity);
        Node *node = cache.buckets[bucket];
        while (node && node->value != value)
            node = node->hash_next;

        if (node) {
            ++hits;
            unlink_node(&cache, node);
        } else {
            if (used < cache.capacity) {
                node = &nodes[used++];
            } else {
                /* Reuse the least recently used node. */
                node = cache.tail;
                size_t old_bucket = bucket_for(node->value, cache.capacity);
                Node **link = &cache.buckets[old_bucket];
                while (*link != node)
                    link = &(*link)->hash_next;
                *link = node->hash_next;
                unlink_node(&cache, node);
            }
            node->value = value;
            node->hash_next = cache.buckets[bucket];
            cache.buckets[bucket] = node;
        }
        prepend(&cache, node);
    }

    printf("%" PRId64 "\n", hits);
    free(nodes);
    free(cache.buckets);
    return EXIT_SUCCESS;
}
