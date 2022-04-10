//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// capture with move semantics
//
//------------------------------------------------------------------------------

#include <iostream>
#include <iterator>
#include <vector>

int main() {
  std::ostream_iterator<int> os{std::cout, " "};
  std::vector v = {1, 2, 3};
  auto out = [w = std::move(v), os] { std::copy(w.begin(), w.end(), os); };
  std::copy(v.begin(), v.end(), os);
  std::cout << std::endl;
  out();
  std::cout << std::endl;
}