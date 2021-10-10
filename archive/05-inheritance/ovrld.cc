#include <iostream>

struct Base {
  virtual int foo(int) { return 10; } 
  virtual int foo(double) { return 20; } 
};

struct Derived : Base {
  using Base::foo;
  int foo(int) override { return 30; }
};

int main() {
  Derived d;
  std::cout << d.foo(1.0) << std::endl;
}

