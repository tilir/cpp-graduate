//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Metaprogramming: factorial example
//
//------------------------------------------------------------------------------

#include <iostream>
#include <utility>

template <int N>
struct fact : std::integral_constant<int, N * fact<N - 1>{}> {};

template <> struct fact<0> : std::integral_constant<int, 1> {};

int main() { std::cout << fact<5>{} << std::endl; }
