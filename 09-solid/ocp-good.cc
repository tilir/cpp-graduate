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
#include <memory>
#include <vector>

struct IDrawable;

struct IScreen {
  virtual void render() const = 0;
  virtual void draw(std::shared_ptr<IDrawable> f) = 0;
  std::ostream &stream() const { return std::cout; }
  virtual ~IScreen() = default;
};

struct IDrawable {
  virtual void draw(const IScreen &s) const = 0;
  virtual ~IDrawable() = default;
};

using ByteStream = IScreen;
void serialize(ByteStream &bs, std::shared_ptr<IDrawable> p) { bs.draw(p); }

struct Vector3D : public IDrawable {
  int x_, y_, z_;
  Vector3D(int x = 0, int y = 0, int z = 0) : x_(x), y_(y), z_(z) {}
  Vector3D &operator+=(const Vector3D &lhs) {
    x_ += lhs.x_;
    y_ += lhs.y_;
    z_ += lhs.z_;
    return *this;
  }

  void draw(const IScreen &s) const override {
    s.stream() << "(" << x_ << ", " << y_ << ", " << z_ << ")";
  }
};

struct Quaternion {
  Vector3D v;
  int w;
};

class Polygon3D : public IDrawable {
  std::vector<Vector3D> vs_;
  using CItt = typename std::vector<Vector3D>::const_iterator;

public:
  Polygon3D(std::initializer_list<Vector3D> il = {}) : vs_(il) {}
  void translate(const Vector3D &t) {
    for (auto &v : vs_)
      v += t;
  }
  void rotate(const Quaternion &q) {
    // for (auto &p : vs_)
    //   p = inverse(q) * p * q;
  }
  void draw(const IScreen &s) const override {
    for (auto v : vs_) {
      v.draw(s);
      s.stream() << "\n";
    }
  }

  CItt begin() const { return vs_.cbegin(); }
  CItt end() const { return vs_.cend(); }
};

class Screen : public IScreen {
  std::vector<std::shared_ptr<IDrawable>> figures_;

public:
  void draw(std::shared_ptr<IDrawable> f) override { figures_.push_back(f); }

  void render() const override {
    for (auto f : figures_)
      f->draw(*this);
  }
};

int main() {
  std::initializer_list<Vector3D> il{{2, 1, 6}, {-3, 7, 4}};
  auto p = std::make_shared<Polygon3D>(il);
  Screen s;
  s.draw(p);
  s.render();
}