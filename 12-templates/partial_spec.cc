//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Example of partial temnplate specialization
//
//-----------------------------------------------------------------------------

#include <iostream>

template <typename T, typename U> struct Foo {
  Foo() { std::cout << 1 << std::endl; }
};

template <typename T> struct Foo<T, T> {
  Foo() { std::cout << 2 << std::endl; }
};

template <typename T> struct Foo<T, int> {
  Foo() { std::cout << 3 << std::endl; }
};

template <typename T, typename U> struct Foo<T *, U *> {
  Foo() { std::cout << 4 << std::endl; }
};

int main() {
  Foo<int, float> mif;       // 1
  Foo<float, float> mff;     // 2
  Foo<float, int> mfi;       // 3
  Foo<int *, float *> mpipf; // 4

#ifdef II
  Foo<int, int> mii; // what do you think?
#endif

#ifdef PII
  Foo<int *, int *> mpipi; // what do you think?
#endif
}
