//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Primes by Erwin Unrih, modern variant, by D. Vandervoorde
//
// compile with clang++ only
// clang++ unruh.cc 2> >(grep error)
//
//------------------------------------------------------------------------------

template <int p, int i> struct is_prime {
  enum {
    prim = (p == 2) || ((p % i) && is_prime<(i > 2 ? p : 0), i - 1>::prim)
  };
};

template <> struct is_prime<0, 0> {
  enum { prim = 1 };
};

template <> struct is_prime<0, 1> {
  enum { prim = 1 };
};

template <int i> struct D { D(void *); };

template <int i> struct CondNull { static int const value = i; };
template <> struct CondNull<0> { static void *value; };
void *CondNull<0>::value = 0;

template <int i> struct Prime_print {
  Prime_print<i - 1> a;
  enum { prim = is_prime<i, i - 1>::prim };
  void f() {
    D<i> d = CondNull < prim ? 1 : 0 > ::value;
    a.f();
  }
};

template <> struct Prime_print<1> {
  enum { prim = 0 };
  void f() { D<1> d = 0; };
};

int main() {
  Prime_print<70> a;
  a.f();
}
