//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// mismatch vs equal exmaple
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main() {
  std::ostream_iterator<int> os(std::cout, " ");
  std::vector<int> a = {1, 2, 4, 9};
  std::vector<int> b = {
      1,
      2,
      4,
      8,
  };
  auto [ita, itb] = std::mismatch(a.begin(), a.end(), b.begin());
  if (ita != a.end() && itb != b.end())
    std::cout << "Not equal\n";

  if (!std::equal(a.begin(), a.end(), b.begin()))
    std::cout << "Not equal\n";
}
