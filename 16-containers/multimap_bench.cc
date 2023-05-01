//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// umap vs ummap access bench
// illustrates that unordered_map<vector> is far better
// https://godbolt.org/z/EPq6b9nd7
// https://quick-bench.com/q/2QpWTQLfj-HUpdn-xa64k26iIGA
//
//------------------------------------------------------------------------------

#include <benchmark/benchmark.h>
#include <concepts>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

static std::random_device rd;
static std::mt19937 rng{rd()};

template <std::integral T> T dice(T min, T max) {
  std::uniform_int_distribution<T> uid(min, max);
  return uid(rng);
}

constexpr int vsz = 10, size = 1000, valmax = 1000;

static void MultiMapAcc(benchmark::State &state) {
  std::unordered_multimap<int, int> u;
  std::vector<int> v(vsz);
  std::generate(v.begin(), v.end(), []() { return dice(0, 10); });
  for (int i = 0; i < size; ++i) {
    auto &&key = v[dice(0, vsz - 1)];
    auto &&value = dice(0, valmax);
    u.insert(std::make_pair(key, value));
  }

  for (auto _ : state) {
    auto &&key = v[dice(0, vsz - 1)];
    auto &&range = u.equal_range(key);
    if (range.first != range.second)
      std::for_each(range.first, range.second, [](auto &&x) {
        auto y = x.second;
        benchmark::DoNotOptimize(y);
      });
  }
  benchmark::DoNotOptimize(u);
}

BENCHMARK(MultiMapAcc);

static void MapAcc(benchmark::State &state) {
  std::unordered_map<int, std::vector<int>> u;
  std::vector<int> v(vsz);
  std::generate(v.begin(), v.end(), []() { return dice(0, 10); });
  for (int i = 0; i < size; ++i) {
    auto &&key = v[dice(0, vsz - 1)];
    auto &&value = dice(0, valmax);
    u.insert(std::make_pair(key, value));
  }

  for (auto _ : state) {
    auto &&key = v[dice(0, vsz - 1)];
    auto &&range = u[key];
    for (auto &&x : range) {
      auto y = x;
      benchmark::DoNotOptimize(y);
    }
  }
  benchmark::DoNotOptimize(u);
}

BENCHMARK(MapAcc);

BENCHMARK_MAIN();