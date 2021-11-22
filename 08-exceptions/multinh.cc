//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Demonstration of problems with multiple inheritance
//
//----------------------------------------------------------------------------

#include <iostream>
#include <stdexcept>

struct my_exc1 : std::exception {
  char const *what() const noexcept override { return "exc1"; }
};

struct my_exc2 : std::exception {
  char const *what() const noexcept override { return "exc2"; }
};

struct your_exc3 : my_exc1, my_exc2 {};

int main() {
  try {
    throw your_exc3();
  } catch (std::exception const &e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cerr << "whoops!\n";
  }
}
