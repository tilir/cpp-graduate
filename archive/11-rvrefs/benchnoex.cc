//-----------------------------------------------------------------------------
//
// Benchmark for noexcept operator=(T&&)
//
//-----------------------------------------------------------------------------
//
// Being noexcept is really important
// Results on my computer:
// $ bench 10000000
// Linear fill with noexcept: 2965
// Linear fill without noexcept: 8176
//
//-----------------------------------------------------------------------------

#include <chrono>
#include <iostream>
#include <vector>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

constexpr int NSZ = 100;

struct MovableNoExcept {
  std::vector<int> arr{NSZ};
  MovableNoExcept() = default;
  ~MovableNoExcept() = default;
  MovableNoExcept(const MovableNoExcept &rhs) : arr(rhs.arr) {}
  MovableNoExcept(MovableNoExcept &&rhs) noexcept : arr(std::move(rhs.arr)) {}
  MovableNoExcept &operator=(const MovableNoExcept &rhs) {
    arr = rhs.arr;
    return *this;
  }
  MovableNoExcept &operator=(MovableNoExcept &&rhs) noexcept {
    arr = std::move(rhs.arr);
    return *this;
  }
};

struct MovableExcept {
  std::vector<int> arr{NSZ};
  MovableExcept() = default;
  ~MovableExcept() = default;
  MovableExcept(const MovableExcept &rhs) : arr(rhs.arr) {}
  MovableExcept(MovableExcept &&rhs) : arr(std::move(rhs.arr)) {}
  MovableExcept &operator=(const MovableExcept &rhs) {
    arr = rhs.arr;
    return *this;
  }
  MovableExcept &operator=(MovableExcept &&rhs) {
    arr = std::move(rhs.arr);
    return *this;
  }
};

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " nfill" << std::endl;
    return -1;
  }

  int nfill = std::stoi(argv[1]);

  std::vector<MovableNoExcept> v_noexc;
  std::vector<MovableExcept> v_exc;

  auto tstart = high_resolution_clock::now();
  for (int i = 0; i < nfill; ++i)
    v_noexc.emplace_back();
  auto tfin = high_resolution_clock::now();

  std::cout << "Linear fill with noexcept: "
            << duration_cast<milliseconds>(tfin - tstart).count() << std::endl;

  tstart = high_resolution_clock::now();
  for (int i = 0; i < nfill; ++i)
    v_exc.emplace_back();
  tfin = high_resolution_clock::now();

  std::cout << "Linear fill without noexcept: "
            << duration_cast<milliseconds>(tfin - tstart).count() << std::endl;
}
