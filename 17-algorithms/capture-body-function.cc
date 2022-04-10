//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// capture in class body with pseudo-methods
//
//------------------------------------------------------------------------------

#include <functional>
#include <iostream>

using VVTy = std::function<void(void)>;

struct Foo {
  int x = 5;
#if 0
  VVTy xplusbad1 = [x] mutable { x += 3; } (); // FAIL
  VVTy xplusbad2 = [&x] { x += 3; } (); // FAIL
#endif
  VVTy xplus1 = [=] { x += 3; };    // not exactly ok (deprecated in C++20)
  VVTy xplus2 = [&] { x += 3; };    // OK but why?
  VVTy xplus3 = [this] { x += 3; }; // OK
};

int main() {
  Foo f;
  f.xplus1();
  f.xplus2();
  f.xplus3();
  std::cout << f.x << std::endl;
}