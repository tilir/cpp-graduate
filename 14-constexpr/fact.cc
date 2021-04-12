#include <iostream>

template <unsigned N> using icu = std::integral_constant<unsigned, N>;

template <unsigned N> struct factorial : icu<N * factorial<N - 1>::value> {};

template <> struct factorial<0> : icu<1> {};

int main() { std::cout << factorial<5>::value << std::endl; }
