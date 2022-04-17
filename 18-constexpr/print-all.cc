//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// print all, constexpr if edition
//
//------------------------------------------------------------------------------

#include <iostream>

template <typename T, typename... Args> void print_all(T first, Args... args) {
  std::cout << first;
  if constexpr (sizeof...(args) > 0) {
    std::cout << ", ";
    print_all(args...);
  }
}

int main() {
  print_all(1, 1.0, 1u);
  std::cout << std::endl;
}
