#include <iostream>

template <typename T> class scoped_ptr {
  T *ptr;

public:
  scoped_ptr(T *ptr) : ptr{ptr} {}
  ~scoped_ptr() { delete ptr; }
  T **operator&() { return &ptr; }
  T operator*() { return *ptr; }
};

int main() {
  scoped_ptr<int> pt{new int{42}};

  std::cout << *pt << " " << **&pt << std::endl;
}