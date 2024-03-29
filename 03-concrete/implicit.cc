//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of implicit ctor
//
//----------------------------------------------------------------------------

template <typename T> struct Quat {
  T x_ = 0, y_ = 0, z_ = 0, w_ = 0;
  Quat(T x) : x_(x) {} // T -> Quat<T>
};

int foo(Quat<int> q) { return q.x_; }

int main() {
  return foo(42); // ok, Quat<int> implicitly constructed
}
