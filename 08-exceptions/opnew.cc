//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  operator new overload
//
//----------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <new>
#include <stdexcept>

void *operator new(std::size_t n) {
  void *p = std::malloc(n);
  if (!p)
    throw std::bad_alloc{};

#ifdef USECOUT
  // Probably bad idea. Why?
  std::cout << "Alloc: " << p << ", size is " << n << "\n";
#else
  std::printf("Alloc: %p, size is %zu\n", p, n);
#endif
  return p;
}

void operator delete(void *mem) noexcept {
  printf("Free: %p\n", mem);
  free(mem);
}

int main() {
  std::list<int> l;
  l.push_back(42);
}
