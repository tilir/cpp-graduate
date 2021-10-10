#include <cstdio>

int foo() { return 42; }

int main() {
  int x;
  int &rx = x;
  const int &l = foo();
  printf("%p\n", &l);
  printf("%p %p\n", &x, &rx);
}
