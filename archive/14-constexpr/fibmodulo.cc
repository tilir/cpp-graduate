// g++ -ftemplate-depth=10000 fibmodulo.cc

#include <iostream>

template <unsigned N> using icu = std::integral_constant<unsigned, N>;

// double "recursion" is ok at compile time
template <unsigned N, unsigned Mod>
struct Fibonacci
    : icu<(Fibonacci<N - 1, Mod>::value + Fibonacci<N - 2, Mod>::value) % Mod> {
};

template <unsigned Mod> struct Fibonacci<0, Mod> : icu<0> {};
template <unsigned Mod> struct Fibonacci<1, Mod> : icu<1> {};

// double recursion is bad at runtime
unsigned fib_naive_mod(unsigned n, unsigned m) {
  if (n == 0)
    return 0;
  if (n == 1)
    return 1;
  return (fib_naive_mod(n - 1, m) + fib_naive_mod(n - 2, m)) % m;
}

enum { N = 44, M = 1000 };

int main() {
  // ct calc
  std::cout << Fibonacci<N, M>::value << std::endl;

  // rt calc
  std::cout << fib_naive_mod(N, M) << std::endl;
}
