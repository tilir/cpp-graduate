#include <iostream>

int main() {
  int a = 2;
  double b = 2.8;
  short c = a * b;
  std::cout << std::dec << a << " * " << b << " = " << c << std::endl;

  unsigned short x = 0xFFFE, y = 0xEEEE;
  unsigned short v = x * y;
  unsigned w = x * y;
  unsigned long long z = x * y;

  std::cout << std::hex << x << " * " << y << " = " << v << std::endl;
  std::cout << std::hex << x << " * " << y << " = " << w << std::endl;
  std::cout << std::hex << x << " * " << y << " = " << z << std::endl;
}
