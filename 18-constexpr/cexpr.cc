//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Idea of const-exprness. Const here just not work
//
//------------------------------------------------------------------------------

#include <climits>
#include <iostream>

template <typename T> struct my_numeric_limits;

template <> struct my_numeric_limits<char> {
  static constexpr size_t max() { return CHAR_MAX; }
};

char arr0[my_numeric_limits<char>::max()];

constexpr size_t MAXSZ = my_numeric_limits<char>::max();
char arr[MAXSZ];

int main() {
  std::cout << sizeof(arr0) << std::endl;
  std::cout << sizeof(arr) << std::endl;
}
