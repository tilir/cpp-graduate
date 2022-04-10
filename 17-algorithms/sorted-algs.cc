//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Algorithms on sorted ranges
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 42, 42, 42, 91, 92, 93, 94};
  auto itl = std::lower_bound(v.begin(), v.end(), 42);
  auto itu = std::upper_bound(v.begin(), v.end(), 42);
  auto itp = std::equal_range(v.begin(), v.end(), 42);
  std::cout << *itl << " " << *itu << std::endl;
  std::cout << *itp.first << " " << *itp.second << std::endl;
}