//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Demonstration of parent inversion: model is inside the class
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

void draw(int x, Screen &out, size_t position) {
  out << std::string(position, ' ') << x << "\n";
}

void draw(Vector3D v, Screen &out, size_t position) {
  out << std::string(position, ' ') << "(" << v.x << ", " << v.y << ", " << v.z
      << ")\n";
}

class Drawable {
  struct IDrawable {
    virtual ~IDrawable() = default;
    virtual std::unique_ptr<IDrawable> copy_() const = 0;
    virtual void draw_(Screen &, size_t) const = 0;
  };

  struct DrawableInt final : IDrawable {
    int data_;
    DrawableInt(int x) : data_(std::move(x)) {}
    std::unique_ptr<IDrawable> copy_() const override {
      return std::make_unique<DrawableInt>(*this);
    }
    void draw_(Screen &out, size_t position) const override {
      ::draw(data_, out, position);
    }
  };

  struct DrawableVector3D final : IDrawable {
    Vector3D data_;
    DrawableVector3D(Vector3D x) : data_(std::move(x)) {}
    std::unique_ptr<IDrawable> copy_() const override {
      return std::make_unique<DrawableVector3D>(*this);
    }
    void draw_(Screen &out, size_t position) const override {
      ::draw(data_, out, position);
    }
  };

  std::unique_ptr<IDrawable> self_;

public:
  Drawable(int x) : self_(std::make_unique<DrawableInt>(std::move(x))) {}

  Drawable(Vector3D x)
      : self_(std::make_unique<DrawableVector3D>(std::move(x))) {}

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

int main() {
  Model document;
  document.push_back(0);
  document.push_back(Vector3D{2, 1, 6});
  document.push_back(Vector3D{-3, 7, 4});
  document.push_back(3);
  draw(document, std::cout, 0);
}
