//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// integer square root, metaprogramm
//
//------------------------------------------------------------------------------

#include <iostream>
#include <utility>

template <int N, int L = 1, int H = N, int mid = (L + H + 1) / 2>
struct Sqrt : std::integral_constant<
                  int, std::conditional_t<(N < mid * mid), Sqrt<N, L, mid - 1>,
                                          Sqrt<N, mid, H>>{}> {};

template <int N, int S>
struct Sqrt<N, S, S, S> : std::integral_constant<int, S> {};

int main() { std::cout << Sqrt<16>{} << std::endl; }