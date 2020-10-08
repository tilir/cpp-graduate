#include <iostream>

#ifdef CERR
int operator-(int x) {
  std::cout << "MINUS!" << std::endl;
  return x;
}
#endif

template <typename T> T operator-(T x) {
  std::cout << "MINUS!" << std::endl;
  return x;
}

int main() {
  int a = 42;
  return -a;
}