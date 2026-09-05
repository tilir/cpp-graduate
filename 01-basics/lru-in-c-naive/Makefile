CC ?= cc
CPPFLAGS ?=
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
LDFLAGS ?=
LDLIBS ?=
PYTHON ?= python3

.PHONY: all test clean

all: lru

lru: main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

test: lru
	$(PYTHON) tests/test_lru.py ./lru

clean:
	$(RM) lru
