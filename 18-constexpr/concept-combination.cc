//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Example of combined concept
//
//------------------------------------------------------------------------------

#include <concepts>
#include <iostream>
#include <utility>

template <class T, class U>
concept WeaklyEqualityComparableWith = requires(
    const std::remove_reference_t<T> &t, const std::remove_reference_t<U> &u) {
  // clang-format off
  { t == u } -> std::convertible_to<bool>;
  { t != u } -> std::convertible_to<bool>;
  { u == t } -> std::convertible_to<bool>;
  { u != t } -> std::convertible_to<bool>;
  // clang-format on
};

template <typename T, typename U>
requires WeaklyEqualityComparableWith<T, U>
bool foo(T x, U y) {
  if ((x == y) && (y != x)) {
    std::cout << "comparison is weak" << std::endl;
    return false;
  }
  std::cout << "comparison is strong" << std::endl;
  return true;
}

struct W {};

bool operator==(W, int) { return true; }
bool operator==(int, W) { return true; }
bool operator!=(W, int) { return true; }
bool operator!=(int, W) { return true; }

int main() {
#ifdef BAD
  struct S {};
  auto w = foo(S{}, 1); // wrong
#endif
  auto c1 = foo(1, W{}); // corr
  auto c2 = foo(1, 2);   // corr
}
