#include <iostream>

int main() {
  int n;
  while (std::cin >> n) {
    std::cout << n << std::endl;
    std::cin.ignore(); // eating Enter hit
  }
}
