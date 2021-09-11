//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example for LRU cache in C++: simple driver program
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>

#include "cache.hpp"

// slow get page imitation
int slow_get_page_int(int key) { return key; }

int main() {
  int hits = 0;
  int n;
  size_t m;

  std::cin >> m >> n;
  assert(std::cin.good());
  caches::cache_t<int> c{m};

  for (int i = 0; i < n; ++i) {
    int q;
    std::cin >> q;
    assert(std::cin.good());
    if (c.lookup_update(q, slow_get_page_int))
      hits += 1;
  }

  std::cout << hits << std::endl;
}
