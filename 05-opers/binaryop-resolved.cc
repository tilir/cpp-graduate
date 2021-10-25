//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Binary operator for templated class: something like a solution
//
//----------------------------------------------------------------------------

template<typename T> struct Quat {
  T x_, y_, z_, w_;
 
  // implicit cast T -> Quat
  Quat(T x = 0, T y = 0, T z = 0, T w = 0) : x_(x), y_(y), z_(z), w_(w) {}

  // operator += in class
  Quat& operator+=(const Quat& rhs) {
    x_ += rhs.x_; y_ += rhs.y_; z_ += rhs.z_; w_ += rhs.w_;
    return *this;
  }

#ifndef NOFAIL
  // operator+ in class
  Quat operator+(const Quat& rhs) { Quat tmp(*this); tmp += rhs; return tmp; }
#endif
};

#ifdef NOFAIL
template <typename T>
Quat<T> operator+(Quat<T> lhs, Quat<T> rhs) { Quat<T> tmp(lhs); tmp += rhs; return tmp; }

template <typename T>
Quat<T> operator+(T lhs, Quat<T> rhs) { Quat<T> tmp(lhs); tmp += rhs; return tmp; }

template <typename T>
Quat<T> operator+(Quat<T> lhs, T rhs) { Quat<T> tmp(lhs); tmp += rhs; return tmp; }
#endif

int main() {
  Quat<int> q;
  Quat<int> p = q + 1;
  Quat<int> t = 1 + q; // FAIL anyway
}
