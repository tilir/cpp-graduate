//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Demonstration of stack unwinding
//
//----------------------------------------------------------------------------

#include <iostream>

struct UnwShow {
  int n_;
  long t = 0xDEADBEEF;
  UnwShow(int n) : n_(n) { std::cout << "ctor: " << n_ << "\n"; }
  ~UnwShow() { std::cerr << "dtor: " << n_ << "\n"; }
};

void foo(int n) {
  UnwShow s{n};

  // odr-use to materialize
  std::cout << "Addr of " << n << ": " << &s << std::endl;
  if (n == 0) {
    std::cout << "throw\n";
    throw 1;
  }
  foo(n - 1);
}

int main() {
  try {
    foo(5);
  } catch (int n) {
    std::cout << "catch\n";
  }
}
