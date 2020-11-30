#include <iostream>

using std::cout;
using std::endl;

struct Base {
  virtual ~Base() {}
};

struct Derived : Base {};

int main() {
  try {
    throw 1;
  } catch (long) {
    cout << "long" << endl;
  } catch (int) {
    cout << "int" << endl;
  }

  try {
    throw 1;
  } catch (const double &) {
    cout << "const double &" << endl;
  } catch (const int &) {
    cout << "const int &" << endl;
  }

  try {
    throw 1;
  } catch (const int &) {
    cout << "first" << endl;
  } catch (int) {
    cout << "second" << endl;
  }

  try {
    throw 1;
  } catch (int) {
    cout << "new first" << endl;
  } catch (const int &) {
    cout << "new second" << endl;
  }

  try {
    throw Derived();
  } catch (Base &) {
    cout << "Base" << endl;
  }

  try {
    throw new Derived();
  } catch (Base *b) {
    cout << "Base*" << endl;
    delete b;
  }
}
