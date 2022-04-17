//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// binary suffix, constexpr edition
//
//------------------------------------------------------------------------------

#include <array>
#include <iostream>

// binary suffix
template <char... Chars> constexpr int operator"" _binary() {
  std::array<int, sizeof...(Chars)> arr{Chars...};
  int sum = 0;
  for (auto c : arr)
    switch (c) {
    case '0':
      sum = sum * 2;
      break;
    case '1':
      sum = sum * 2 + 1;
      break;
    default:
      throw "Unexpected symbol";
    }
  return sum;
}

int main() { std::cout << 11001010_binary << std::endl; }
