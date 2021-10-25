#pragma once

int foo();
int bar();

inline int only_inline() { return 1; }
static int only_static() { return 2; }
static inline int static_inline() { return 3; }

namespace {
int anon_namespace() { return 4; }
} // namespace

#ifdef NOODR
int odr_violation() { return 5; }
#endif
