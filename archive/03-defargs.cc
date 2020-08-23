#include <iostream>

void foo(int x, int y = 0);
void bar(int x, int y);
void buz();

int main() {
  foo(3);
  // bar(3); // error!
  buz();
}

void foo(int x, int y = 0) {
  std::cout << "foo: " << x << " " << y << std::endl;
}

void bar(int x, int y = 0) {
  std::cout << "bar: " << x << " " << y << std::endl;
}

void buz() {
  bar(3); // ok
}
