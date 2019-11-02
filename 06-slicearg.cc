#include <iostream>

struct A {
  int a_;
  A(int a) : a_(a) {}
  virtual A& operator=(const A& rhs) { 
    a_ = rhs.a_;
    return *this;
  }
  virtual ~A() {}
};

struct B : public A {
  int b_;
  B(int b) : A(b / 2), b_(b) {}
  B& operator=(const A& rhs) override {
    const B& brhs = static_cast<const B&>(rhs);
    a_ = brhs.a_;
    b_ = brhs.b_;
    return *this;
  }
};

std::ostream& operator<< (std::ostream& os, const A& a) {
  os << a.a_;
  return os;
}

std::ostream& operator<< (std::ostream& os, const B& b) {
  os << b.a_ << " " << b.b_;
  return os;
}

void foo(A a) {
  std::cout << a << std::endl;
}

int main() {
  B b1(10);
  B b2(8);
  foo(b1);
  foo(b2);
}