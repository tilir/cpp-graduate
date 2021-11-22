//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  operator new custom overload
//
//----------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include <list>

void *operator new(std::size_t n) {
  void *p = malloc(n);
  if (!p)
    throw std::bad_alloc{};
  printf("Alloc: %p, size is %zu\n", p, n);
  return p;
}

void operator delete(void *mem) noexcept {
  printf("Free: %p\n", mem);
  free(mem);
}

struct Widget {
  static void *operator new(std::size_t n);
  static void operator delete(void *mem) noexcept;
  int n[4];
};

void *Widget::operator new(std::size_t n) {
  void *p = malloc(n);
  if (!p)
    throw std::bad_alloc{};
  printf("Custom alloc: %p, size is %zu\n", p, n);
  return p;
}

void Widget::operator delete(void *mem) noexcept {
  printf("Custom free: %p\n", mem);
  free(mem);
}

int main() {
  std::list<int> l;
  l.push_back(42);
  Widget *w = new Widget;
  delete w;
}
