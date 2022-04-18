//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Partition to classes with requires-expression and SFINAE comparison
//
//------------------------------------------------------------------------------

#include <iostream>
#include <utility>

#if defined(SFINAE)

// SFINAE: not working

template <typename T, typename = std::enable_if_t<(sizeof(T) > 4)>>
void foo(T x) {
  std::cout << "For large" << std::endl;
}

template <typename T, typename = std::enable_if_t<(sizeof(T) <= 4)>>
void foo(T x) {
  std::cout << "For small" << std::endl;
}

#else

// constraints: ok

template <typename T> requires(sizeof(T) > 4) void foo(T x) {
  std::cout << "For large" << std::endl;
}

template <typename T> requires(sizeof(T) <= 4) void foo(T x) {
  std::cout << "For small" << std::endl;
}

#endif

int main() {
  foo('a');
  foo(1LL);
}