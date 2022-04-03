// https://quick-bench.com/q/af_lxuL36JnWXCG7GNXs59GjpTg

#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <numeric>
#include <random>
#include <unordered_map>

namespace chr = std::chrono;

constexpr int NMEASURES = 1;
constexpr int SZ = 10000000;
constexpr int NKEYS = 10;

static std::random_device rd;
static std::mt19937 rng{rd()};

int dice(int min, int max) {
  std::uniform_int_distribution<int> uid(min, max);
  return uid(rng);
}

class Timer {
  chr::time_point<chr::high_resolution_clock> tstart, tfin;

public:
  void start() { tstart = chr::high_resolution_clock::now(); }
  void fin() { tfin = chr::high_resolution_clock::now(); }
  auto msec() {
    return chr::duration_cast<chr::milliseconds>(tfin - tstart).count();
  }
};

int collatz(int n) {
  int c = 0;
  while (n > 1) {
    c += 1;
    if ((n % 2) == 0)
      n = n / 2;
    else
      n = 3 * n + 1;
  }
  return c;
}

static void Sequenced() {
  std::unordered_multimap<int, int> m;
  for (int i = 0; i < SZ; ++i)
    m.emplace(i % NKEYS, i);  
  for (int state = 0; state < NMEASURES; ++state) {
    for (int i = 0; i < NKEYS; ++i) {
      auto [begin, end] = m.equal_range(i);
      std::for_each(begin, end, [](auto &&elt) { 
        elt.second = collatz(elt.second); });
    } 
  }
}

static void Parunseq() {
  std::unordered_multimap<int, int> m;
  for (int i = 0; i < SZ; ++i)
    m.emplace(i % NKEYS, i);
  for (int state = 0; state < NMEASURES; ++state) {
    for (int i = 0; i < NKEYS; ++i) {
      auto [begin, end] = m.equal_range(i);       
      std::for_each(std::execution::par_unseq, begin, end, [](auto &&elt) {
        elt.second = collatz(elt.second); });
    }
  }
}

int main() {
  Timer t;
  t.start();
  Sequenced();
  t.fin();
  std::cout << "Sequenced: " << t.msec() << std::endl;

  t.start();
  Parunseq();
  t.fin();
  std::cout << "Par + Unseq: " << t.msec() << std::endl;
}
