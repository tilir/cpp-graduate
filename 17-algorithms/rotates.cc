//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// naive repose with ad-hoc rotates
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main() {
  std::ostream_iterator<int> osit{std::cout, " "};
  std::vector<int> v(10);
  std::iota(v.begin(), v.end(), 1);
  std::copy(v.begin(), v.end(), osit);
  std::cout << std::endl;
  auto first = std::find(v.begin(), v.end(), 3);
  auto last = std::find(v.begin(), v.end(), 6);
  auto pos = std::find(v.begin(), v.end(), 8);
  std::rotate(first, last, pos);
  std::copy(v.begin(), v.end(), osit);
  std::cout << std::endl;
  std::iota(v.begin(), v.end(), 1);
  first = std::find(v.begin(), v.end(), 7);
  last = std::find(v.begin(), v.end(), 9);
  pos = std::find(v.begin(), v.end(), 2);
  std::rotate(pos, first, last);
  std::copy(v.begin(), v.end(), osit);
  std::cout << std::endl;
}
