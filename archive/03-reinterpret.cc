#include <iostream>

void basic_ones() {
  int x;
  double y = 1.0;
  x = static_cast<int>(y);
  std::cout << "x = " << x << std::endl;
  const int *p = &x;
  int *q = const_cast<int *>(p);
  uintptr_t uq = reinterpret_cast<uintptr_t>(q);
  std::cout << "uq = " << uq << std::endl;
}

void reint_vs() {
  char c = 161;
  std::cout << "char # " << static_cast<int>(c) << std::endl;
  // std::cout << "char # " << reinterpret_cast<int>(c) << std::endl;

  int i;
  const int *p = &i;
  std::cout << "int: " << *(const_cast<int *>(p)) << std::endl;
  // std::cout << "int: " << *(reinterpret_cast<int*>(p)) << std::endl;
}

int main() {
  basic_ones();
  reint_vs();
}
