//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Demonstration of noexcept(false)
//
//----------------------------------------------------------------------------

#include <iostream>
#include <stdexcept>

#ifdef BAD
struct T {
  ~T() {
    throw std::runtime_error("");
  }
};

void test0() {
  try {
    T t;
  }
  catch (std::runtime_error &e) {
    std::cerr << "Exception catched\n";
  }
  std::cout << "Success\n";
}
#endif

struct S {
  ~S() noexcept(false) {
    if (std::uncaught_exceptions())
      std::cerr << "Dtor called in unwinding\n";
    throw std::runtime_error("");
  }
};

void test1() {
  try {
    S s;
  }
  catch (std::runtime_error &e) {
    std::cerr << "Exception catched\n";
  }
  std::cerr << "Success\n";
}

void test2() {
  try {
    S s;
    throw std::runtime_error("");
  }
  catch (std::runtime_error &e) {
    std::cerr << "Exception catched\n";
  } 
  std::cerr << "Success\n";
}

int main() {
#ifdef BAD
    std::cerr << "test0: ";
    test0();
#endif    
    std::cerr << "test1: ";
    test1();
    std::cerr << "test2: ";
    test2();
}