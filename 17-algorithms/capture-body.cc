//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Capture in class body
//
//------------------------------------------------------------------------------

#include <functional>
#include <iostream>

struct Foo {
  int x = 5;
  void xplus() {
#if 0
    [x] mutable { x += 3; } (); // FAIL
    [&x] { x += 3; } (); // FAIL
#endif
    [=] { x += 3; }();    // not exactly ok (deprecated in C++20)
    [&] { x += 3; }();    // OK
    [this] { x += 3; }(); // OK
  }
};

int main() {
  Foo f;
  f.xplus();
  std::cout << f.x << std::endl;
}