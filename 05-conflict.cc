#include <iostream>

struct Foo;
struct Bar;

struct Foo {
  Foo() {}
  Foo(const Bar &) { std::cout << "Ctor Bar -> Foo" << std::endl; }
};

struct Bar {
  Bar() {}
  Bar(const Foo &) {}
  operator Foo() {
    std::cout << "Op Bar -> Foo" << std::endl;
    return Foo{};
  }
};

void foo(const Foo &f) { std::cout << "Done" << std::endl; }

int main() {
  Bar b;
  foo(b);
}
