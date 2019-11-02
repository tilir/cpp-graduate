#include <iostream>

struct Base {
  Base() { unsafe(); } 
  void unsafe() { doIt(); } // PVC invocation
  virtual void doIt() = 0;
  virtual ~Base() {}
};

struct Derived : public Base {
  void doIt() override {
    std::cout << "Derived::doit" << std::endl;
  }
};

int main() {
  Derived d;  // PVC appears
}