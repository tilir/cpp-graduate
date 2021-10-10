#include <iostream>

struct Foo {
  Foo(long x = 0) {}
  operator long() { return 42; }
};

void foo(int x) { std::cout << "int: " << x << std::endl; }

void foo(Foo x) { std::cout << "Foo: " << x << std::endl; }

int main() {
  Foo f;
  foo(f);
  foo(+f);

  long l;
  foo(l);
}