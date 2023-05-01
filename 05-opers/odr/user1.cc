#include "header.h"

int foo() {
  int x = static_inline();
  x += only_inline();
  x += only_static();
  x += anon_namespace();
#ifdef NOODR
  x += odr_violation();
#endif
  return x;
}
