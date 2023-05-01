//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Effective quick sort with "operator ()"
//
//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <ctime>

struct myless {
  bool operator()(const int &lhs, const int &rhs) const { return lhs > rhs; }
};

int main(int argc, char **argv) {
  size_t nelts, i;
  int *narr;
  clock_t start, fin;

  printf("Hello from C++ qsort bench op overload\n");

  if (argc != 2) {
    printf("usage: %s <number-of-elements>\n", argv[0]);
    abort();
  }

  nelts = strtol(argv[1], NULL, 10);
  assert(nelts > 0);

  narr = (int *)calloc(nelts, sizeof(int));
  assert(narr);

  for (i = 0; i < nelts; ++i)
    narr[i] = rand() % nelts;

  start = clock();

  // myless comp;
  // std::sort(narr, narr + nelts, [](auto lhs, auto rhs) { return lhs > rhs;
  // });

  myless lesspred;
  bool b = lesspred(1, 2);
  assert(!b);
  std::sort(narr, narr + nelts, lesspred);

  fin = clock();
  printf("Elapsed: %lg seconds\n", (double)(fin - start) / CLOCKS_PER_SEC);
}
