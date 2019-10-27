#include <iostream>
#include <utility>

template <typename T, typename U> std::pair<T, U> operator-(std::pair<T, U> p) {
  return {-p.first, -p.second};
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, std::pair<T, U> p) {
  os << p.first << " " << p.second;
  return os;
}

int main() {
  std::pair<int, int> p{10, 20}, q;
  q = -p;

  std::cout << q << std::endl;
}
