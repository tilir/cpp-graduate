//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Finally example
// Inspired by https://www.youtube.com/watch?v=eG5suWcHI8M
//
//------------------------------------------------------------------------------

#include <functional>
#include <iostream>

namespace V1 {
struct Finally {
  std::function<void()> action_;
  explicit Finally(std::function<void()> action) : action_(std::move(action)) {}
  ~Finally() { action_(); }
};
} // namespace V1

namespace V2 {
template <typename ActTy> inline auto Finally(ActTy fn) {
  struct Finally_impl {
    ActTy act;
    explicit Finally_impl(ActTy action) : act(std::move(action)) {}
    ~Finally_impl() { act(); }
  };
  return Finally_impl(std::move(fn));
}
} // namespace V2

namespace V3 {
template <typename ActTy> struct Finally {
  ActTy act;
  explicit Finally(ActTy action) : act(std::move(action)) {}
  ~Finally() { act(); }
};
} // namespace V3

int main() {
  int x = 0;
  V1::Finally close_x1{[&] { std::cout << x << std::endl; }};
  auto close_x2 = V2::Finally([&] { std::cout << x << std::endl; });
  V3::Finally close_x3{[&] { std::cout << x << std::endl; }};
  x = 42;
}