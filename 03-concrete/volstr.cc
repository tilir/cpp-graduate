//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Volatile string demo: string class have no volatile methods
// compile with: g++ volstr.cc
//
//-----------------------------------------------------------------------------

#include <iostream>
#include <string>

int main() {
  volatile std::string s{"Hello"};

  char c = s[2];
  s += "world";
  std::cout << s << std::endl;
  std::cout << s.c_str() << std::endl;
}