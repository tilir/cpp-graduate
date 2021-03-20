//-----------------------------------------------------------------------------
//
// Benchmark for naive copy vs memcpy
//
//-----------------------------------------------------------------------------
//
// $ bench 1000000000
// memcpy: 1883
// naive copy: 3167
//
// Problem is obvious: naive copy copies naively and this is ~7 times slower
//
// with -DSTANDARD
// std copy: 1834
// magic happens!
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <random>
#include <vector>

#ifndef STANDARD
#define NAIVE
#endif

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const int IMAX = 9;

template <typename InputIt, typename OutputIt>
OutputIt naive_copy(InputIt first, InputIt last, OutputIt d_first) {
  while (first != last)
    *d_first++ = *first++;
  return d_first;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " nelts" << std::endl;
    return -1;
  }

  auto nelts = std::stoi(argv[1]);
  auto *arr = new int[nelts];
  auto *arrcopy = new int[nelts];
  auto *arrcopy2 = new int[nelts];

  std::random_device rd;
  std::default_random_engine reng(rd());
  std::uniform_int_distribution<int> dist(0, IMAX);

  std::generate(arr, arr + nelts, [&dist, &reng] { return dist(reng); });

  auto tstart = high_resolution_clock::now();
  memcpy(arrcopy, arr, nelts * sizeof(int));
  auto tfin = high_resolution_clock::now();

  std::cout << "memcpy: " << duration_cast<milliseconds>(tfin - tstart).count()
            << std::endl;

  tstart = high_resolution_clock::now();
#ifdef NAIVE  
  naive_copy(arr, arr + nelts, arrcopy2);
#else
  std::copy(arr, arr + nelts, arrcopy2);
  // basically the same for:
  // std::copy_n(arr, nelts, arrcopy2);
#endif  
  tfin = high_resolution_clock::now();

#ifdef NAIVE  
  std::cout << "naive copy: "
#else
  std::cout << "std copy: "
#endif  
            << duration_cast<milliseconds>(tfin - tstart).count() << std::endl;

  // sanity: do we have mismatch (we shall not)
  auto mism = std::mismatch(arrcopy, arrcopy + nelts, arrcopy2);
  if (mism.first != arrcopy + nelts || mism.second != arrcopy2 + nelts) {
    std::cout << "mismatch: " << *mism.first << " vs " << *mism.second << std::endl;
    std::cout << "at: " << mism.first - arrcopy << std::endl;
  }

  delete[] arrcopy2;
  delete[] arrcopy;
  delete[] arr;
}
