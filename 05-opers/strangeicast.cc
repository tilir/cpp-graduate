//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Strange implicit cast behavior
//
//----------------------------------------------------------------------------

#include <iostream>
#include <string>

struct S {
  S(std::string) {}
  S(std::wstring) {}
};

bool operator==(S lhs, S rhs) { return true; }

int main() {
  std::string s = "foo";
  std::wstring t = L"bar";
  if (s == t)
    std::cout << "WAT?\n";
}
