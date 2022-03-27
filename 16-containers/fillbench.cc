//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// vector vs deque fill bmk
//
//------------------------------------------------------------------------------

#include <benchmark/benchmark.h>
#include <deque>
#include <vector>

static void DequeFill(benchmark::State &state) {
  for (auto _ : state) {
    std::deque<int> d;
    for (int i = 0; i < 100000; ++i)
      d.push_back(i);
    benchmark::DoNotOptimize(d);
  }
}
BENCHMARK(DequeFill);

static void VectorFill(benchmark::State &state) {
  for (auto _ : state) {
    std::vector<int> v;
    for (int i = 0; i < 100000; ++i)
      v.push_back(i);
    benchmark::DoNotOptimize(v);
  }
}
BENCHMARK(VectorFill);

BENCHMARK_MAIN();