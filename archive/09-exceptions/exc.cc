#include <iostream>

using std::cout;
using std::endl;

struct UnwShow {
  long t = 0xDEADBEEF;
  UnwShow() { cout << "ctor" << endl; }
  ~UnwShow() { std::cerr << "dtor" << endl; }
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
  foo(5);
  /*
    try {
    } catch (...) {
    }
    */
}
