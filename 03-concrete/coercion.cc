//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Coercion demo: coercion is not copy. No output in second "Copy:" block.
// compile with: g++ coercion.cc
//
//-----------------------------------------------------------------------------

#include <iostream>

template <typename T> struct Copyable {
  Copyable() {}
  Copyable(const Copyable &c) {
    std::cout << "Copyable copy ctor" << std::endl;
  }
};

template <typename T> struct Coercible {
  Coercible() {}
  template <typename U> Coercible(const Coercible<U> &c) {
    std::cout << "Coercible coercion ctor" << std::endl;
  }
};

int main() {
  std::cout << "Copy:" << std::endl;

  Copyable<void> a;
  Copyable<void> b{a};

  std::cout << "Copy:" << std::endl;

  Coercible<void> c;
  Coercible<void> d{c};

  std::cout << "Coercion:" << std::endl;

  Coercible<int> e{c};
}
