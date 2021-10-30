//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of slicing problem
//
//----------------------------------------------------------------------------

#include <iostream>

struct A {
  int a_;
  A(int a) : a_(a) {}
};

struct B : public A {
  int b_;
  B(int b) : A(b / 2), b_(b) {}
};

std::ostream &operator<<(std::ostream &os, const B &b) {
  os << b.a_ << " " << b.b_;
  return os;
}

int main() {
  B b1(10); // a_ = 5, b_ = 10
  B b2(8);  // a_ = 4, b_ = 8
  A &a_ref = b2;
  a_ref = b1; // a_ = 5, b_ = 8, WAT??
  std::cout << b2 << std::endl;
}