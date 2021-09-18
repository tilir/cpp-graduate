//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example for reference binding: displaying addresses
//
//----------------------------------------------------------------------------

#include <iostream>

int foo() { return 42; }

int main() {
  int x;
  int &rx = x;
  const int &l = foo();
  std::cout << &l << " " << &x << " " << &rx << std::endl;
}
