//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Capture with rename
//
//------------------------------------------------------------------------------

#include <iostream>

int main() {
  int z = 5;
  auto t = [k = z](auto x, auto y) { return x < y * k; };
  std::cout << std::boolalpha << t(6, 3) << std::endl;
}