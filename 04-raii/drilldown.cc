//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of drill-down behavior of -> operator
//
//----------------------------------------------------------------------------

#include <iostream>

template <typename T> class scoped_ptr {
  T *ptr_;

public:
  scoped_ptr(T *ptr) : ptr_{ptr} {}
  ~scoped_ptr() { delete ptr_; }
  scoped_ptr(const scoped_ptr &rhs) : ptr_{rhs.ptr_} {}
  T *operator->() { return ptr_; }
  const T *operator->() const { return ptr_; }
};

struct X {
  int a, b;
};

int main() {
  scoped_ptr<X> pt{new X{2, 3}};
  std::cout << pt->a << std::endl;

  const scoped_ptr<X> ptc{new X{2, 3}};
  std::cout << ptc->a << std::endl;
}
