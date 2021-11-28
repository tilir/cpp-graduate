//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Strict guarantees of new[] regarding dtors
//
//----------------------------------------------------------------------------

#include <iostream>
#include <stdexcept>

int nsucc = 10;
int nctors = 0;
int ndtors = 0;

struct S {
  S() {
    if (nsucc == 0)
      throw std::exception{};
    nctors += 1;
    nsucc -= 1;
  }

  ~S() { ndtors += 1; }
};

int main() {
  try {
    S *sarr = new S[20];
  } catch (std::exception &) {
    std::cout << "Exception catched\n";
  }
  std::cout << "# ctors called: " << nctors << std::endl;
  std::cout << "# dtors called: " << ndtors << std::endl;
}