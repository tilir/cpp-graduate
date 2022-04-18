//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Constexpr inside demo
//
//------------------------------------------------------------------------------

#include <iostream>
#include <vector>

template <typename T> consteval auto ilist_sz(std::initializer_list<T> init) {
#if defined(BAD)
  // BAD (try to remove CE)
  constexpr auto init_sz = init.size();
#else
  auto init_sz = init.size();
#endif
  return init_sz;
}

int main() {
  constexpr auto n = ilist_sz({1, 2, 3});
  std::cout << n << std::endl;
}