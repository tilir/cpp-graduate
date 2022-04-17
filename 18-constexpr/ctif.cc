//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Constexpr if application
//
//------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <utility>

template <typename T> auto length(T value) {
  if constexpr (std::is_integral<T>::value)
    return sizeof(T);
  else
    return value.length();
}

int main() {
  int a = 5;
  std::string b = "foo";
  std::cout << length(a) << ' ' << length(b) << std::endl;
}
