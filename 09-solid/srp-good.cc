//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Good code for SRP: polygon exposes explicit iterator
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
  using CItt = typename std::vector<Vector3D>::const_iterator;

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

  CItt begin() const { return vs_.cbegin(); }
  CItt end() const { return vs_.cend(); }
};

void draw(Screen &s, const Polygon3D &p) {
  for (auto v : p) {
    ::draw(s, v);
    std::cout << "\n";
  }
}

void serialize(ByteStream &bs, const Polygon3D &p) { draw(bs, p); }

int main() {
  Polygon3D p = {{2, 1, 6}, {-3, 7, 4}};
  draw(std::cout, p);
}
