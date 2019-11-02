#include <iostream>

struct Matrix {
  template <typename T>
  virtual void pow(T x) {
    std::cout << x << std::endl;
  }

  virtual ~Matrix() {}
};

int main() {
  Matrix m;
  m.pow(1.5);
}