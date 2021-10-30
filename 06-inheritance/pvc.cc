//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of PVC invocation
//  Compile: g++ pvc.cc -DPVCDIAG
//  Compile: g++ pvc.cc -DPVC
//  Then run and you will have PVC error
//
//----------------------------------------------------------------------------

#include <iostream>

struct Base {
#if defined(PVC)
  Base() { unsafe(); }
#elif defined(PVCDIAG)
  Base() { doIt(); }
#else
  Base() { safe(); }
#endif
  void basesafe() { safe(); }
  virtual void safe() { std::cout << "Base\n"; } // ok invocation
  void unsafe() { doIt(); }                      // PVC invocation
  virtual void doIt() = 0;
  virtual ~Base() = 0;
};

Base::~Base() {}

struct Derived : public Base {
  void safe() override { std::cout << "Derived" << std::endl; }
  void doIt() override { std::cout << "Derived::doit" << std::endl; }
};

int main() {
  Derived d; // PVC appears
  d.basesafe();
}
