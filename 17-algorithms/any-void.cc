//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Empty range for all_of, any_of, none_of
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
  std::vector<int> v; // пустой вектор
  auto id = [](auto &&x) { return x; };
  if (std::all_of(v.begin(), v.end(), id))
    std::cout << "All\n";
  if (std::any_of(v.begin(), v.end(), id))
    std::cout << "Any\n";
  if (std::none_of(v.begin(), v.end(), id))
    std::cout << "None\n";
}
