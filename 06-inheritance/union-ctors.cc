//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of union with ctors and why you shall not do this
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#ifdef WRONG
union Wrong {
  std::string s_;
  std::vector<int> v_;
};
#endif

union U {
  std::string s_;
  std::vector<int> v_;
  U(std::string s) { new (&s_) std::string{s}; }
  U(std::vector<int> v) { new (&v_) std::vector<int>{v}; }
  ~U() {} // here =default will not work
};

struct S {
  bool str_;
  U u_;
  S(std::string s) : str_(true), u_(s) {}
  S(std::vector<int> v) : str_(false), u_(v) {}

  int &operator[](int n) {
    assert(!str_);
    return u_.v_[n];
  }
  operator const char *() {
    assert(str_);
    return u_.s_.c_str();
  }

  ~S() {
    if (str_)
      u_.s_.~basic_string<char>();
    else
      u_.v_.~vector();
  }
};

int main() {
#ifdef WRONG
  Wrong w;
#endif
  std::string s = "Hello";
  std::vector<int> v = {0, 1, 2, 3, 4};
  S t1{s};
  S t2{v};
  std::cout << t1 << " " << t2[1] << std::endl;
}
