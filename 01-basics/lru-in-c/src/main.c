#include "lru.h"

#include <stdbool.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/* Parse a whole decimal token without scanf's out-of-range conversions. */
static bool read_integer(int64_t *value) {
    int ch;
    do {
        ch = getchar();
    } while (ch != EOF && isspace((unsigned char)ch));
    bool negative = ch == '-';
    if (ch == '-' || ch == '+')
        ch = getchar();
    if (ch < '0' || ch > '9')
        return false;
    uint64_t magnitude = 0;
    uint64_t limit = negative ? (uint64_t)INT64_MAX + 1 : INT64_MAX;
    do {
        unsigned digit = (unsigned)(ch - '0');
        if (magnitude > (limit - digit) / 10)
            return false;
        magnitude = magnitude * 10 + digit;
        ch = getchar();
    } while (ch >= '0' && ch <= '9');
    if (ferror(stdin) || (ch != EOF && !isspace((unsigned char)ch)))
        return false;
    if (negative && magnitude == (uint64_t)INT64_MAX + 1)
        *value = INT64_MIN;
    else
        *value = negative ? -(int64_t)magnitude : (int64_t)magnitude;
    return true;
}

/* A real loader could fetch data from disk or the network. */
static Page slow_get_page(PageId key) {
    return key;
}

int main(void) {
    int64_t capacity, count;
    if (!read_integer(&capacity) || !read_integer(&count) ||
        capacity < 0 || count < 0) {
        fprintf(stderr, "Expected nonnegative cache size and element count.\n");
        return EXIT_FAILURE;
    }
    /* A stream of count elements cannot fill more than count slots. */
    uint64_t limit = (uint64_t)(capacity < count ? capacity : count);
    LruCache *cache = limit <= SIZE_MAX ? lru_create((size_t)limit) : NULL;
    if (!cache) {
        fprintf(stderr, "Cannot allocate cache.\n");
        return EXIT_FAILURE;
    }
    int64_t hits = 0;
    for (int64_t i = 0; i < count; ++i) {
        PageId key;
        if (!read_integer(&key)) {
            fprintf(stderr, "Expected integer at position %" PRId64 ".\n", i + 1);
            lru_destroy(cache);
            return EXIT_FAILURE;
        }
        Page page;
        LruResult result = lookup_update(cache, key, &slow_get_page, &page);
        if (result != LRU_HIT && result != LRU_MISS) {
            fprintf(stderr, "Cannot fetch page (error %d).\n", (int)result);
            lru_destroy(cache);
            return EXIT_FAILURE;
        }
        hits += result == LRU_HIT;
    }
    lru_destroy(cache);
    if (printf("%" PRId64 "\n", hits) < 0 || fflush(stdout) == EOF)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
