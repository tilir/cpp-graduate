#include <iostream>

struct Base {
  // Base() { doIt(); }
  Base() { safe(); }
  void basesafe() { safe(); }
  virtual void safe() { std::cout << "Base\n"; } // ok invocation
  void unsafe() { doIt(); } // PVC invocation
  virtual void doIt() = 0;
  virtual ~Base() = 0;
};

Base::~Base() {
}

struct Derived : public Base {
  void safe() override { std::cout << "Derived" << std::endl; }
  void doIt() override { std::cout << "Derived::doit" << std::endl; }
};

int main() {
  Derived d; // PVC appears
  d.basesafe();
}
