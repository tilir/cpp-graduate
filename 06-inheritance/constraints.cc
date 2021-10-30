//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Template virtual method -- can it exist?
//
//----------------------------------------------------------------------------

#include <iostream>

struct Matrix {
  template <typename T> virtual void pow(T x) { std::cout << x << std::endl; }

  virtual ~Matrix() {}
};

int main() {
  Matrix m;
  m.pow(1.5);
}