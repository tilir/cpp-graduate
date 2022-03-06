//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Range-based for example
//
//-----------------------------------------------------------------------------

#include <iostream>
#include <utility>

namespace fibs {

struct fibonacci {};

struct fibit {
  int cur = 1, prev = 0;
  fibit &operator++() {
    prev = std::exchange(cur, cur + prev);
    return *this;
  }
  int operator*() { return cur; }
  auto operator<=>(const fibit &) const = default;
};

fibit begin(fibonacci) { return fibit{}; }
fibit end(fibonacci) { return fibit{89, 55}; }

} // namespace fibs

int main() {
  for (auto f : fibs::fibonacci{}) {
    std::cout << f << " ";
  }
  std::cout << std::endl;
}
