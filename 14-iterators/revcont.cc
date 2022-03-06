//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// reversion adapter
//
//-----------------------------------------------------------------------------

#include <iostream>
#include <vector>

template <typename T> struct reversion_wrapper { T &iterable; };

template <typename T> auto begin(reversion_wrapper<T> w) {
  return std::rbegin(w.iterable);
}

template <typename T> auto end(reversion_wrapper<T> w) {
  return std::rend(w.iterable);
}

template <typename T> reversion_wrapper<T> reverse_cont(T &&iterable) {
  return {iterable};
}

int main() {
  std::vector v = {2, 3, 5, 7, 11};
  for (auto elt : v)
    std::cout << elt << std::endl;
  for (auto elt : reverse_cont(v))
    std::cout << elt << std::endl;
}
