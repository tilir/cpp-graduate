//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Both op- redefined inside and outside class
//
//----------------------------------------------------------------------------

#include <iostream>

template <typename T> struct Quat {
  T x, y, z, w;

  Quat operator-() const {
    std::cout << "in class\n";
    return Quat{-x, -y, -z, -w};
  }
};

template <typename T> Quat<T> operator-(const Quat<T> &arg) {
  std::cout << "out class\n";
  return Quat<T>{-arg.x, -arg.y, -arg.z, -arg.w};
}

int main() {
  Quat<float> q{1, 2, 3, 4};
  Quat<float> p = -q;
  Quat<float> r = operator-(q);
  Quat<float> s = q.operator-();
}
