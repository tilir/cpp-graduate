//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  LValue annotations example: compilation error
//
//----------------------------------------------------------------------------

#include <iostream>

struct S {
  int n = 0;
  int &access() & { return n; }
};

int main() {
  S x;
  int &y = x.access();   // ok
  int &z = S{}.access(); // compilation fail
  std::cout << y << " " << z << std::endl;
}
