//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Bad code violating OCP: switch too ugly
//
//----------------------------------------------------------------------------

#include <iostream>
#include <vector>

struct IFigure {
  enum class Shape { VECTOR, POLYGON, CIRCLE };
  virtual Shape shape() const = 0;
  virtual ~IFigure() = default;
};

struct IScreen {
  virtual void draw(const IFigure &f) = 0;
  virtual void render() const = 0;
  virtual ~IScreen() = default;
};

using ByteStream = IScreen;
void serialize(ByteStream &bs, const IFigure &p) { bs.draw(p); }

struct Vector3D : public IFigure {
  int x_, y_, z_;
  Vector3D(int x = 0, int y = 0, int z = 0) : x_(x), y_(y), z_(z) {}
  Vector3D &operator+=(const Vector3D &lhs) {
    x_ += lhs.x_;
    y_ += lhs.y_;
    z_ += lhs.z_;
    return *this;
  }

  Shape shape() const override { return IFigure::Shape::VECTOR; }
};

struct Quaternion {
  Vector3D v;
  int w;
};

class Polygon3D : public IFigure {
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
  Shape shape() const override { return IFigure::Shape::POLYGON; }

  CItt begin() const { return vs_.cbegin(); }
  CItt end() const { return vs_.cend(); }
};

class Screen : public IScreen {
  std::vector<const IFigure *> figures_;

  void drawVector(const Vector3D &v) const {
    std::cout << "(" << v.x_ << ", " << v.y_ << ", " << v.z_ << ")";
  }

  void drawPolygon(const Polygon3D &p) const {
    for (auto v : p) {
      drawVector(v);
      std::cout << "\n";
    }
  }

public:
  void draw(const IFigure &f) override { figures_.push_back(&f); }

  void render() const override {
    for (auto f : figures_) {
      switch (f->shape()) {
      case IFigure::Shape::POLYGON:
        drawPolygon(*static_cast<const Polygon3D *>(f));
        break;
      case IFigure::Shape::VECTOR:
        drawVector(*static_cast<const Vector3D *>(f));
        break;
      }
    }
  }
};

int main() {
  Polygon3D p = {{2, 1, 6}, {-3, 7, 4}};
  Screen s;
  s.draw(p);
  s.render();
}