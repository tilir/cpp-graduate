//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// generalized repose (with some problems inside)
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

template <typename RandIt> void repose(RandIt first, RandIt last, RandIt pos) {
  // [3, 6) to position before 3 means leave it in place
  // [3, 6) to position before 6 means leave it in place as well
  if (first == pos || last == pos)
    return;
  // not clear what it means to move [3, 6) to position before 5?
  // this position is itself inside interval
  if (last < pos && first > pos)
    throw std::runtime_error("Can not move to position inside interval");
  if (first < pos)
    std::rotate(first, last, pos);
  else
    std::rotate(pos, first, last);
}

int main() {
  std::ostream_iterator<int> osit{std::cout, " "};
  std::vector<int> v(10);

  // 1. [3, 6) --> pos before 8
  std::iota(v.begin(), v.end(), 1);
  std::copy(v.begin(), v.end(), osit);
  std::cout << std::endl;
  auto first = std::find(v.begin(), v.end(), 3);
  auto last = std::find(v.begin(), v.end(), 6);
  auto pos = std::find(v.begin(), v.end(), 8);
  repose(first, last, pos);
  std::copy(v.begin(), v.end(), osit);
  std::cout << std::endl;

  // 2. [7, 9) --> pos before 2
  std::iota(v.begin(), v.end(), 1);
  first = std::find(v.begin(), v.end(), 7);
  last = std::find(v.begin(), v.end(), 9);
  pos = std::find(v.begin(), v.end(), 2);
  repose(first, last, pos);
  std::copy(v.begin(), v.end(), osit);
  std::cout << std::endl;
}
