//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Name hiding puzzle: which pow?
//
//----------------------------------------------------------------------------

#include <iostream>

struct Matrix {
  virtual void pow(int x) { std::cout << x << std::endl; }

  virtual void pow(double x) { std::cout << x << std::endl; }

  virtual ~Matrix() {}
};

struct SparseMatrix : public Matrix {
  void pow(int x) override { std::cout << x << std::endl; }
};

int main() {
  SparseMatrix d;
  d.pow(1.5);
}