//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Example simplified from Arthur O'Dwyers zip range to illustrate key concept
// of proxy classes for operator->() and overall iterator development
//
// original: https://quuxplusone.github.io/blog/2019/02/06/arrow-proxy/
//
//-----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <vector>

#include "ziprange.hpp"

int main() {
  std::vector<bool> b = {false, false, true, false};
  std::vector<int> k = {1, 2, 3, 4};
  std::vector<double> v = {4.5, 3.5, 2.5, 1.5};

  // 1. assignment
  auto zit = itertools::make_zip_iterator(k.begin(), b.begin());
  assert(zit->first == k.front());
  (*zit).first = 42;
  assert(zit->first == 42);
  zit->second = true;
  assert(b.front() == true);

  // 2. diapasone iterating
  for (auto &&both : itertools::make_zip_range(k, v))
    std::cout << both.first << " " << both.second << "\n";

    // 3. wrong iterators don't work
#ifdef BAD
  auto osit = std::ostream_iterator<int>{std::cout};
  auto wit = itertools::make_zip_iterator(osit, b.begin());
#endif
}
