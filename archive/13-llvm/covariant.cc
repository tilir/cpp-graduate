#include <iostream>
#include <memory>

struct Base {
  virtual ~Base() {}
};

struct Derived : public Base {
  Derived() { std::cout << "Derived\n"; }
  ~Derived() { std::cout << "~Derived\n"; }
};

std::unique_ptr<Base> get() {
  // ok, covariant
  return std::unique_ptr<Derived>{ new Derived{} };
}

int main() {
  std::unique_ptr<Base> b = get(); // ok, rvalue
  std::unique_ptr<Derived> d { new Derived{} };

  b = std::move(d);
  std::cout << "---\n";
}