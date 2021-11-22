//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Demonstration of slicing inside exception handling
//
//----------------------------------------------------------------------------

#include <iostream>

using std::cout;
using std::endl;

struct Base {
  virtual ~Base() { cout << "~Base" << endl; }
};

struct Derived : Base {
  ~Derived() { cout << "~Derived" << endl; }
};

int main() {
  try {
    throw Derived();
  }
#if defined(CORR)
  catch (Base &b) {
  }
#else
  catch (Base b) {
  }
#endif
}
