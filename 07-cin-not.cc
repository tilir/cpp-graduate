#include <iostream>

int main() {
  int n;
  while (!std::cin >> n) { // ERROR HERE!
    std::cout << n << std::endl;
    std::cin.ignore();
  }
}
