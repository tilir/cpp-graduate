//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Metaprogramming: fibonacci example
//
//------------------------------------------------------------------------------

#include <iostream>
#include <utility>

template <int N>
struct fibonacci
    : std::integral_constant<int, fibonacci<N - 1>{} + fibonacci<N - 2>{}> {};

template <> struct fibonacci<1> : std::integral_constant<int, 1> {};

template <> struct fibonacci<0> : std::integral_constant<int, 0> {};

int main() { std::cout << fibonacci<8>{} << std::endl; }
