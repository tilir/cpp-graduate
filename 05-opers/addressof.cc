//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  example of explicit adressoff
//
//----------------------------------------------------------------------------

#include <iostream>

struct MyInt {
  int x_;
  MyInt(int x = 0) : x_(x) {}
  int operator&() { return 42; } // because I can
};

int main() {
  MyInt a = 5;
  std::cout << "Seemingly " << &a << " but really " << std::addressof(a)
            << std::endl;
}
