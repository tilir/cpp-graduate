//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// RVO demo
// compile with: g++ rvo.cc
// compile with: g++ -fno-elide-constructors rvo.cc
//
//-----------------------------------------------------------------------------

#include <iostream>

using std::cout;
using std::endl;

struct foo {
  foo() { cout << "foo::foo()" << endl; }
  foo(const foo &) { cout << "foo::foo(const foo&)" << endl; }
  ~foo() { cout << "foo::~foo()" << endl; }
};

foo bar() {
  foo local_foo;
  return local_foo;
}

int main() { foo f = bar(); }
