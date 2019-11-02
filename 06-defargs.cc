#include <iostream>

struct Base {
  virtual int foo(int a = 14) { return a; }
  virtual ~Base() {}
};

struct Derived : public Base {
  int foo(int a = 42) override { return a; }
};

int main() {
  Base *pb = new Derived{};  
  std::cout << pb->foo() << std::endl;
  delete pb;
}
