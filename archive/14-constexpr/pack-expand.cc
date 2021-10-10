#include <iostream>

#include "concrete-functions.h"

using std::cout;
using std::endl;

template <typename... T> void foo(T... args) { f(h(args...) + h(args)...); }

template <typename... T> void bar(T... args) { f(h(args, args...)...); }

int main(void) {
  cout << "foo:" << endl;
  foo(1, 2, 3);
  cout << "bar:" << endl;
  bar(1, 2, 3);
}
