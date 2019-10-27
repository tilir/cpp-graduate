#include <iostream>

int main() {
  int a = 3, b = 4, c = 5;
  a += b *= c -= 1;
  std::cout << a << " " << b << " " << c << std::endl;
}
