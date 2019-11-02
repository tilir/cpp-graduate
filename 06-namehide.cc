#include <iostream>

struct Matrix {
  virtual void pow(int x) {
    std::cout << x << std::endl;
  }
  
  virtual void pow(double x) {
    std::cout << x << std::endl;
  }
};

struct SparseMatrix : public Matrix {
  void pow(int x) override {
    std::cout << x << std::endl;
  }
};

int main() {
  SparseMatrix d;
  d.pow(1.5);
}