//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of most vex parsing
//
//----------------------------------------------------------------------------

#include <iostream>

struct list_t {};
struct myclass_t {
  int x = 42;
  myclass_t(list_t, list_t) {}
};

int main() {
  myclass_t m1(list_t(), list_t());
  myclass_t m2{list_t(), list_t()};

  std::cout << m1.x << std::endl; // WAT?
  std::cout << m2.x << std::endl;
}
