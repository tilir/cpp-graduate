//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// SFINAE triada for equality comparable
//
//------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <utility>

template <typename T, typename U, typename = void>
struct is_equality_comparable : std::false_type {};

template <typename T, typename U>
struct is_equality_comparable<
    T, U, std::void_t<decltype(std::declval<T>() == std::declval<U>())>>
    : std::true_type {};

template <typename T, typename U>
bool is_equality_comparable_v = is_equality_comparable<T, U>::value;

int main() {
  std::cout << std::boolalpha;
  std::cout << is_equality_comparable_v<std::string, int> << std::endl;
  std::cout << is_equality_comparable_v<long &, char> << std::endl;
}