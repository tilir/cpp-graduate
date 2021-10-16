//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example for integral promotions
//
//----------------------------------------------------------------------------

#include <iostream>

int main() {
  int a = 2;
  double b = 2.8;
  short c = a * b;
  std::cout << std::dec << a << " * " << b << " = " << c << std::endl;

  unsigned short x = 0xFFFE, y = 0xEEEE;
  unsigned short v = x * y;
  std::cout << std::hex << x << " * " << y << " = " << v << std::endl;

  unsigned w = x * y;
  std::cout << std::hex << x << " * " << y << " = " << w << std::endl;

  // in unsigned long long shall be represented value of signed int
  unsigned long long z = x * y;
  std::cout << std::hex << x << " * " << y << " = " << z << std::endl;
}
