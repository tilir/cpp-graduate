//-----------------------------------------------------------------------------
//
// Benchmark for push vs emplace
//
//-----------------------------------------------------------------------------
//
// Using emplace looks cleaner and may be faster
//
// $ bench 1000000
// Linear fill with push: 1466
// Linear fill with emplace: 1121
//
//-----------------------------------------------------------------------------

#include <chrono>
#include <iostream>
#include <list>
#include <vector>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

constexpr int NSZ = 100;

struct Heavy {
  std::vector<int> v;
  Heavy(int n) : v(n) {}
  Heavy(const Heavy &rhs) : v(rhs.v) {}
  Heavy &operator=(const Heavy &rhs) {
    v = rhs.v;
    return *this;
  }

  // inefective move-ctor
  Heavy(Heavy &&rhs) : v(rhs.v) {}
  Heavy &operator=(Heavy &&rhs) = delete;
  ~Heavy() = default;
};

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " nfill" << std::endl;
    return -1;
  }

  int nfill = std::stoi(argv[1]);

  std::list<Heavy> v_push;
  std::list<Heavy> v_emplace;

  auto tstart = high_resolution_clock::now();
  for (int i = 0; i < nfill; ++i)
    v_push.push_back(Heavy{NSZ});
  auto tfin = high_resolution_clock::now();

  std::cout << "Linear fill with push: "
            << duration_cast<milliseconds>(tfin - tstart).count() << std::endl;

  tstart = high_resolution_clock::now();
  for (int i = 0; i < nfill; ++i)
    v_emplace.emplace_back(NSZ);
  tfin = high_resolution_clock::now();

  std::cout << "Linear fill with emplace: "
            << duration_cast<milliseconds>(tfin - tstart).count() << std::endl;
}
