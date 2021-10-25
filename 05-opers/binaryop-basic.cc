//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Binary operator: symmetric for non-template class
//
//----------------------------------------------------------------------------

struct Quat {
  int x_, y_, z_, w_;
 
  // implicit cast int -> Quat
  Quat(int x = 0, int y = 0, int z = 0, int w = 0) : x_(x), y_(y), z_(z), w_(w) {}

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
Quat operator+(Quat lhs, Quat rhs) { Quat tmp(lhs); tmp += rhs; return tmp; }
#endif

int main() {
  Quat q;
  Quat p = q + 1;
  Quat t = 1 + q; // FAIL
}
