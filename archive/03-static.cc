#include <iostream>

struct Sometype {
  int x;
  static int y; // declaration
  void foo() {
    x += 3;
    y += 3;
  }
  static void bar() {
    // x += 3; // FAIL
    y += 3;
  }
};

int Sometype::y; // definition

int main() {
  Sometype t, u;
  Sometype::y = 10;
  t.x = 10;
  u.x = 15;
  t.foo();
  u.foo();
  Sometype::bar();
  std::cout << Sometype::y << std::endl;
}