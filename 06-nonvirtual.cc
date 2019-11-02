#include <iostream>

struct Base {
  Base() { doIt(); } 
  virtual void doIt() {
    std::cout << "Base::doit" << std::endl;
  }
  virtual ~Base() {}
};

struct Derived : public Base {
  void doIt() override {
    std::cout << "Derived::doit" << std::endl;
  }
};

int main() {
  Derived d;  // Base::doIt
}