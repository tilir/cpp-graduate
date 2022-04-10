//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Different capture falvors
//
//------------------------------------------------------------------------------

#include <iostream>

int main() {
  int a = 0, b = 1, c = 2;
  auto fval = [a, b](int x) { return a + b * x; };
  auto fvalm = [a, b](int x) mutable {
    a = a + b * x;
    return a;
  };
  auto fref = [&a, &b](int x) {
    a = a + b * x;
    return a;
  };

  std::cout << fval(5) << std::endl;
  std::cout << fvalm(5) << std::endl;
  std::cout << fref(5) << std::endl;

  a = 10;

  std::cout << fval(5) << std::endl;
  std::cout << fvalm(5) << std::endl;
  std::cout << fref(5) << std::endl;

  auto faval = [=](int x) { return a + b * x; };
  auto faref = [&](int x) {
    a += b * x;
    return a;
  };
  auto favalb = [=, &b](int x) { return a + b * x; };
  auto farefa = [&, a](int x) {
    b += a * x;
    return b;
  };
  auto freval = [la = a](int x) { return la + x; };
  auto freref = [&la = a](int x) {
    la += x;
    return la;
  };
}