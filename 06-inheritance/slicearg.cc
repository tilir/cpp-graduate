//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of slicing with arguments
//
//----------------------------------------------------------------------------

#include <iostream>

struct A {
  int a_;
  A(int a) : a_(a) {}
  virtual void dump(std::ostream &os) const { os << a_; }
  virtual ~A() {}
};

struct B : public A {
  int b_;
  B(int b) : A(b / 2), b_(b) {}
  void dump(std::ostream &os) const override { os << a_ << " " << b_; }
};

std::ostream &operator<<(std::ostream &os, const A &a) {
  a.dump(os);
  return os;
}

void foo(A a) { std::cout << a << std::endl; }

void bar(A &a) { std::cout << a << std::endl; }

int main() {
  B b1(10);
  foo(b1);
  bar(b1);
}