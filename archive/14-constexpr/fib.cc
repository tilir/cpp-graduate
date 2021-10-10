#include <iostream>

template <unsigned N> using icu = std::integral_constant<unsigned, N>;

template <unsigned N>
struct Fibonacci : icu<Fibonacci<N - 1>::value + Fibonacci<N - 2>::value> {};

template <> struct Fibonacci<0> : icu<0> {};
template <> struct Fibonacci<1> : icu<1> {};

int main() { std::cout << Fibonacci<10>::value << std::endl; }
