//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Bad code violating SRP: polygon does too much
//
//----------------------------------------------------------------------------

#include <iostream>
#include <vector>

using Screen = std::ostream;
using ByteStream = std::ostream;

struct Vector3D {
  int x, y, z;
  Vector3D &operator+=(const Vector3D &lhs) {
    x += lhs.x;
    y += lhs.y;
    z += lhs.z;
    return *this;
  }
};

struct Quaternion {
  Vector3D v;
  int w;
};

void draw(Screen &s, Vector3D v) {
  s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

class Polygon3D {
  std::vector<Vector3D> vs_;

public:
  Polygon3D(std::initializer_list<Vector3D> il) : vs_(il) {}
  void translate(const Vector3D &t) {
    for (auto &v : vs_)
      v += t;
  }
  void rotate(const Quaternion &q) {
    // for (auto &p : vs_)
    //   p = inverse(q) * p * q;
  }
  void draw(Screen &s) const {
    for (auto v : vs_) {
      ::draw(s, v);
      std::cout << "\n";
    }
  }
  void serialize(ByteStream &bs) const { draw(bs); }
};

int main() {
  Polygon3D p = {{2, 1, 6}, {-3, 7, 4}};
  p.draw(std::cout);
}
