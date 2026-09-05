CC ?= cc
CPPFLAGS ?=
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
LDFLAGS ?=
LDLIBS ?=
PYTHON ?= python3

LIB_SOURCES = src/list.c src/hash_map.c src/lru.c
OBJECTS = $(LIB_SOURCES:.c=.o) src/main.o

.PHONY: all test clean
all: lru

lru: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

src/%.o: src/%.c
	$(CC) $(CPPFLAGS) -Iinclude $(CFLAGS) -MMD -MP -c -o $@ $<

tests/test_components: tests/test_components.c $(LIB_SOURCES) $(wildcard include/*.h)
	$(CC) $(CPPFLAGS) -Iinclude $(CFLAGS) $(LDFLAGS) -o $@ tests/test_components.c $(LIB_SOURCES) $(LDLIBS)

test: lru tests/test_components
	./tests/test_components
	$(PYTHON) tests/test_lru.py ./lru

clean:
	$(RM) lru $(OBJECTS) $(OBJECTS:.o=.d) tests/test_components

-include $(OBJECTS:.o=.d)
