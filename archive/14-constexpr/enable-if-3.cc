#include <iostream>
#include <utility>

using std::cout;
using std::enable_if;
using std::endl;

template <typename T> void foo(typename enable_if<(sizeof(T) > 4), T>::type x) {
  cout << x << " greater then 4" << endl;
}

template <typename T>
void foo(typename enable_if<(sizeof(T) <= 4), T>::type x) {
  cout << x << " less then 4" << endl;
}

int main() {
  char c = 'A';
  double d = 42.0;

  foo(c);
  foo(d);
}
