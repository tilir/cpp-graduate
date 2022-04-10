//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// copy backward
// try change dpos to pos to see UB (don't do at home)
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main() {
  std::ostream_iterator<int> os(std::cout, " ");
  std::vector<int> a(10);
  std::iota(a.begin(), a.end(), 1);
  std::copy(a.begin(), a.end(), os);
  std::cout << std::endl;

  auto first = std::find(a.begin(), a.end(), 1);
  auto last = std::next(first, 5);
  auto pos = std::find(a.begin(), a.end(), 4);
  auto dpos = std::next(pos, 5);
  std::copy_backward(first, last, dpos);
  std::copy(a.begin(), a.end(), os);
  std::cout << std::endl;
}
