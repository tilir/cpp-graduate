#include <compare>
#include <iostream>

struct MyInt {
  int x_;
  MyInt(int x = 0) : x_(x) {}
  std::strong_ordering operator<=>(const MyInt &rhs) { return x_ <=> rhs.x_; }
};

int main() {
  MyInt a = 5, b = 6;
  std::cout << (b > a) << std::endl;
  std::cout << (b <= a) << std::endl;
}