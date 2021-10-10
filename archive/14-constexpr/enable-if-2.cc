#include <iostream>
#include <utility>

using std::cout;
using std::enable_if;
using std::endl;

template <typename T> typename enable_if<(sizeof(T) > 4), void>::type foo(T x) {
  cout << x << " greater then 4" << endl;
}

template <typename T>
typename enable_if<(sizeof(T) <= 4), void>::type foo(T x) {
  cout << x << " less then 4" << endl;
}

int main() {
  char c = 'A';
  double d = 42.0;

  foo(c);
  foo(d);
}
