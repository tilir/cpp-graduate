#include <iostream>

template <typename T> class scoped_ptr {
  T *ptr;

public:
  scoped_ptr(T *ptr) : ptr{ptr} {}
  ~scoped_ptr() { delete ptr; }
  T *operator->() { return ptr; }
};

struct X {
  int a, b;
};

int main() {
  scoped_ptr<X> pt{new X{2, 3}};
  std::cout << pt->a << std::endl;
}
