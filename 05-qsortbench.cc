#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <ctime>

int less(const int &lhs, const int &rhs) {
  return lhs > rhs;
}

int main(int argc, char **argv) {
  size_t nelts, i;
  int *narr;
  clock_t start, fin;

  printf("Hello from C++ qsort bench function\n");

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

  std::sort(narr, narr + nelts, less);

  fin = clock();
  printf("Elapsed: %lg seconds\n", (double)(fin - start) / CLOCKS_PER_SEC);
}
