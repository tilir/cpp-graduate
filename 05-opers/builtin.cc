//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  operator- for everything
//
//----------------------------------------------------------------------------

#include <iostream>

#ifdef CERR
int operator-(int x) {
  std::cout << "MINUS!" << std::endl;
  return x;
}
#endif

template <typename T> T operator-(T x) {
  std::cout << "MINUS!" << std::endl;
  return x;
}

int main() {
  int a = 42;
  std::cout << -a << std::endl;

#ifdef CERR2
  std::cout << operator-(a) << std::endl;  
#endif
}