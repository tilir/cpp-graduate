//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Consteval and constinit
//
//------------------------------------------------------------------------------

#include <iostream>
#include <vector>

// compile time only
consteval int ctsqr(int n) { return n * n; }

// OK, global constinit
constinit int g = ctsqr(100);

int main() {
  constexpr int c = ctsqr(100);

#ifdef BAD
  // FAIL: cannot assign constexpr (i.e. const) variable
  c += 1;
#endif

#ifdef BAD
  // FAIL: function consteval, x not CT known
  int x = 100;
  int y = ctsqr(x);
#endif

#ifdef BAD
  // FAIL: constinit can not be local
  constinit int d = ctsqr(100);
#endif

  // OK to do this in runtime
  g += 1;

  std::cout << c << " " << g << std::endl;
}