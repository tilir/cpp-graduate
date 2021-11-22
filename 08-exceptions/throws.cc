//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Demonstration of different throws
//
//----------------------------------------------------------------------------

#include <iostream>

struct Base {
  virtual ~Base() {}
};

struct Derived : Base {};

enum MyErrs { MY_OK, MYFAL };

enum class MyErrsC : int { MY_OK, MYFAL };

int main() {
  try {
    MyErrs m = MY_OK;
    throw m;
  } catch (int) {
    std::cout << "int\n";
  } catch (MyErrs) {
    std::cout << "MyErrs\n";
  }

  try {
    MyErrsC m = MyErrsC::MY_OK;
    throw m;
  } catch (int) {
    std::cout << "int\n";
  } catch (MyErrsC) {
    std::cout << "MyErrsC\n";
  }

  try {
    throw 1;
  } catch (long) {
    std::cout << "long\n";
  } catch (int) {
    std::cout << "int\n";
  }

  try {
    throw 1;
  } catch (const double &) {
    std::cout << "const double &\n";
  } catch (const int &) {
    std::cout << "const int &\n";
  }

  try {
    throw 1;
  } catch (const int &) {
    std::cout << "first\n";
  } catch (int) {
    std::cout << "second\n";
  }

  try {
    throw 1;
  } catch (int) {
    std::cout << "new first\n";
  } catch (const int &) {
    std::cout << "new second\n";
  }

  try {
    throw Derived();
  } catch (Base) {
    std::cout << "Base sliced\n";
  }

  try {
    throw Derived();
  } catch (Base &) {
    std::cout << "Base ref\n";
  }

  try {
    throw new Derived();
  } catch (Base *b) {
    std::cout << "Base ptr\n";
    delete b;
  }
}
