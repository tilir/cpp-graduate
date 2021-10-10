#include <iostream>

using std::cout;
using std::endl;

template <typename T> void print_all(T last) { std::cout << last; }

template <typename T, typename... Args> void print_all(T first, Args... args) {
  std::cout << first << " ";
  print_all(args...);
}

int main() {
  print_all(1, 1.0, 1u);
  std::cout << std::endl;
}
