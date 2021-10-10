#ifndef CONCRETE_GUARD_
#define CONCRETE_GUARD_

#include <iostream>

using std::cout;
using std::endl;

static inline int f(int x) {
  cout << "f(" << x << ")" << endl;
  return x;
}

static inline int f(int x, int y) {
  cout << "f(" << x << ", " << y << ")" << endl;
  return x + y;
}

static inline int f(int x, int y, int z) {
  cout << "f(" << x << ", " << y << ", " << z << ")" << endl;
  return x + y + z;
}

static inline int f(int x, int y, int z, int w) {
  cout << "f(" << x << ", " << y << ", " << z << ", " << w << ")" << endl;
  return x + y + z + w;
}

static inline int h(int x) {
  cout << "h(" << x << ")" << endl;
  return x;
}

static inline int h(int x, int y) {
  cout << "h(" << x << " " << y << ")" << endl;
  return x + y;
}

static inline int h(int x, int y, int z) {
  cout << "h(" << x << " " << y << " " << z << ")" << endl;
  return x + y + z;
}

static inline int h(int x, int y, int z, int w) {
  cout << "h(" << x << " " << y << " " << z << " " << w << ")" << endl;
  return x + y + z + w;
}

#endif
