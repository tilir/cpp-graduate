//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  example for chained operators
//
//----------------------------------------------------------------------------

#include <iostream>

int main() {
  int a = 3, b = 4, c = 5;
  a += b *= c -= 1;
  std::cout << a << " " << b << " " << c << std::endl;
}
