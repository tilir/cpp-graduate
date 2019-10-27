#include <iostream>

int foo(int x) { return 0; }
int foo(const int &x) { return 1; }

template <typename T> T foo(T x) { return 2; }

int foo(char x) { return 10; }
int foo(short x) { return 11; }
int foo(...) { return 21; }
int foo(int &x) { return 31; }

int main() {
  std::cout << "result: " << foo(10) << std::endl;
  return 0;
}
