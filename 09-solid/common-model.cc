//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Demonstration of parent inversion: complete SRP and OCP satisfied
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using Screen = std::ostream;

struct Vector3D {
  int x, y, z;
};

struct Polygon3D {
  std::vector<Vector3D> vs;
  Polygon3D(std::initializer_list<Vector3D> il) : vs(il) {}
};

void draw(int x, Screen &out, size_t position) {
  out << std::string(position, ' ') << x << "\n";
}

void draw(Vector3D v, Screen &out, size_t position) {
  out << std::string(position, ' ') << "(" << v.x << ", " << v.y << ", " << v.z
      << ")\n";
}

void draw(const Polygon3D &p, Screen &out, size_t position) {
  out << std::string(position, ' ') << "<polygon>" << std::endl;
  for (auto v : p.vs)
    ::draw(v, out, position + 2);
  out << std::string(position, ' ') << "</polygon>" << std::endl;
}

class Drawable {
  struct IDrawable {
    virtual ~IDrawable() = default;
    virtual std::unique_ptr<IDrawable> copy_() const = 0;
    virtual void draw_(Screen &, size_t) const = 0;
  };

  template <typename T> struct DrawableObject final : IDrawable {
    T data_;
    DrawableObject(T x) : data_(std::move(x)) {}
    std::unique_ptr<IDrawable> copy_() const override {
      return std::make_unique<DrawableObject>(*this);
    }
    void draw_(Screen &out, size_t position) const override;
  };

  std::unique_ptr<IDrawable> self_;

public:
  template <typename T>
  Drawable(T x) : self_(std::make_unique<DrawableObject<T>>(std::move(x))) {}

  // copy ctor, move ctor and assignment
public:
  Drawable(const Drawable &x) : self_(x.self_->copy_()) {}
  Drawable(Drawable &&x) noexcept = default;
  Drawable &operator=(Drawable x) noexcept {
    self_ = std::move(x.self_);
    return *this;
  }

public:
  friend void draw(const Drawable &x, Screen &out, size_t position) {
    x.self_->draw_(out, position);
  }
};

using Model = std::vector<Drawable>;

void draw(const Model &x, Screen &out, size_t position) {
  out << std::string(position, ' ') << "<world>" << std::endl;
  for (const auto &e : x)
    draw(e, out, position + 2);
  out << std::string(position, ' ') << "</world>" << std::endl;
}

template <typename T>
void Drawable::DrawableObject<T>::draw_(Screen &out, size_t position) const {
  ::draw(data_, out, position);
}

int main() {
  Model document;
  document.push_back(0);
  document.push_back(Polygon3D{{2, 1, 6}, {-3, 7, 4}});
  document.push_back(document);
  document.push_back(Vector3D{-3, 7, 4});
  draw(document, std::cout, 0);
}