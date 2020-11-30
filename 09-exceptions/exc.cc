#include <iostream>

using std::cout;
using std::endl;

struct UnwShow {
  long t = 0xDEADBEEF;
  UnwShow() { cout << "ctor" << endl; }
  ~UnwShow() { cout << "dtor" << endl; }
};

int foo(int n) {
  UnwShow s;

  // odr-use to materialize
  cout << &s << endl;

  if (n == 0)
    throw 1;
  foo(n - 1);
}

int main() {
  try {
    foo(5);
  } catch (...) {
  }
}
