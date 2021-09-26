//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of overload
//  compile with: g++ overload.cc -DN0 ... -DN15
//
//----------------------------------------------------------------------------

#include <iostream>

#ifndef N0
int foo(int x) { return 0; }
#endif

#ifndef N1
int foo(const int &x) { return 1; }
#endif

#ifndef N2
template <typename T> T foo(T x) { return 2; }
#endif

#ifndef N10
int foo(char x) { return 10; }
#endif

#ifndef N11
int foo(short x) { return 11; }
#endif

#ifndef N12
int foo(const char &x) { return 12; }
#endif

#ifndef N13
int foo(double x) { return 13; }
#endif

#ifndef N14
int foo(...) { return 21; }
#endif

#ifndef N15
int foo(int &x) { return 31; }
#endif

int main() {
  std::cout << "result: " << foo(10) << std::endl;
  return 0;
}
