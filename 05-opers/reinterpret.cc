//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example for different casts
//
//----------------------------------------------------------------------------

#include <iostream>

int main() {
  // static cast
  double y = 1.0;
  int x = static_cast<int>(y);
  std::cout << "x = " << x << std::endl;

  // const cast
  const int *p = &x;
  int *q = const_cast<int *>(p);
  std::cout << "q = " << *q << std::endl;

  // reinterpret cast
  uintptr_t uq = reinterpret_cast<uintptr_t>(q);
  std::cout << "uq = " << uq << std::endl;

  // bug when using reinterpret where const required
  char c = 161;
  std::cout << "char # " << static_cast<int>(c) << std::endl;
#ifdef REINTS
  std::cout << "char # " << reinterpret_cast<int>(c) << std::endl;
#endif

  // bug when using reinterpret where static required
  std::cout << "int: " << *(const_cast<int *>(p)) << std::endl;
#ifdef REINTC
  std::cout << "int: " << *(reinterpret_cast<int *>(p)) << std::endl;
#endif
}
