//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// vector vs deque access bench
//
//------------------------------------------------------------------------------

#include <benchmark/benchmark.h>
#include <deque>
#include <random>
#include <vector>

static std::random_device rd;
static std::mt19937 rng{rd()};

int dice(int min, int max) {
  std::uniform_int_distribution<int> uid(min, max);
  return uid(rng);
}

constexpr int size = 1000;

static void DequeAcc(benchmark::State &state) {
  std::deque<int> d;
  for (int i = 0; i < size; ++i)
    d.push_back(i);
  for (auto _ : state) {
    int j = dice(0, size - 1);
    j = d[j];
    benchmark::DoNotOptimize(j);
  }
}
BENCHMARK(DequeAcc);

static void VectorAcc(benchmark::State &state) {
  std::vector<int> v;
  for (int i = 0; i < size; ++i)
    v.push_back(i);
  for (auto _ : state) {
    int j = dice(0, size - 1);
    j = v[j];
    benchmark::DoNotOptimize(j);
  }
}
BENCHMARK(VectorAcc);

BENCHMARK_MAIN();