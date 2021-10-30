//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of non-virtual default args
//
//----------------------------------------------------------------------------

#include <iostream>

struct Base {
  virtual int foo(int a = 14) { return a; }
  virtual ~Base() {}
};

struct Derived : public Base {
  int foo(int a = 42) override { return 2 * a; }
};

int main() {
  Base *pb = new Derived{};
  std::cout << pb->foo() << std::endl;
  delete pb;
}
