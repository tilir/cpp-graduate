//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Benchmark for push vs emplace
// Using emplace looks cleaner and may be faster
//
// > g++ -O2 -DVISUALIZE benchemplace.cc -o vbench
// > ./vbench 10
// > g++ -O2 benchemplace.cc -o bench
// > ./bench 40000
// Linear fill with push: 1466
// Linear fill with emplace: 1121
//
//-----------------------------------------------------------------------------

#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

constexpr int NSZ = 10000;

#define NOINLINE __attribute__((noinline))

struct Heavy {
  std::vector<int> v;
  Heavy(int n) NOINLINE : v(n) {
#ifdef VISUALIZE
    std::cout << "default-ctor\n";
#endif
  }

  // inefective copy-ctor (with some work inside)
  Heavy(const Heavy &rhs) NOINLINE : v(rhs.v) {
#ifdef VISUALIZE
    std::cout << "copy-ctor\n";
#endif
    for (auto &elt : v)
      elt *= 17;
  }

  // inefective move-ctor (delegate to copy)
  Heavy(Heavy &&rhs) : Heavy(rhs) {}
  Heavy &operator=(const Heavy &rhs) = delete;
  Heavy &operator=(Heavy &&rhs) = delete;
  ~Heavy() = default;
};

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " nfill" << std::endl;
    return -1;
  }

  int nfill = std::stoi(argv[1]);

  std::vector<Heavy> v_push;
  std::vector<Heavy> v_emplace;
  v_push.reserve(nfill);
  v_emplace.reserve(nfill);

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

  // some usage to not opt-away
  long long aggpush = 0;
  for (auto &&p : v_push)
    aggpush += std::accumulate(p.v.begin(), p.v.end(), 0);

  long long aggemplace = 0;
  for (auto &&p : v_emplace)
    aggemplace += std::accumulate(p.v.begin(), p.v.end(), 0);

  if (aggpush != aggemplace)
    std::cout << "Numerical divergence found" << std::endl;
}
