//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// pretty closure name and underlying type for std::function
//
//------------------------------------------------------------------------------

#include <functional>
#include <iostream>

int f(int a) { return a; }

int main() {
  int x = 5;
  std::function<int(int)> fn1 = f, fn2 = [](int a) { return -a; },
                          fn3 = [x](int a) { return x - a; };
  std::cout << fn1.target_type().name() << std::endl
            << fn2.target_type().name() << std::endl
            << fn3.target_type().name() << std::endl;
}