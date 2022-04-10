//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// hello world with lambdas
//
//------------------------------------------------------------------------------

#include <iostream>

int main(int argc, const char **argv) {
  return [argv]() -> int {
    std::cout << "Hello from " << argv[0] << std::endl;
    return 0;
  }();
}
