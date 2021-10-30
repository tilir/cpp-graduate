//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of non-virtual call
//
//----------------------------------------------------------------------------

#include <iostream>

struct Base {
  Base() { doIt(); }
  virtual void doIt() { std::cout << "Base::doit" << std::endl; }
  virtual ~Base() {}
};

struct Derived : public Base {
  void doIt() override { std::cout << "Derived::doit" << std::endl; }
};

int main() {
  Derived d; // Base::doIt
}