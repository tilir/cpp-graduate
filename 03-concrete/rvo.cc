#include <iostream>

using std::cout;
using std::endl;

struct foo {
  foo() { cout << "foo::foo()" << endl; }
  foo(const foo &) { cout << "foo::foo(const foo&)" << endl; }
  ~foo() { cout << "foo::~foo()" << endl; }
};

foo bar() {
  foo local_foo;
  return local_foo;
}

int main() { foo f = bar(); }
