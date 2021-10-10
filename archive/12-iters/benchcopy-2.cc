//-----------------------------------------------------------------------------
//
// Benchmark for to show that non-naive copy may be no worse
//
//-----------------------------------------------------------------------------
//
// $ bench 1000000000
// memcpy: 1899
// non-naive copy: 1893
//
// Task here: is non-naive copy implementation good enough?
// Can you see problems?
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <list>
#include <random>
#include <vector>

#ifndef STANDARD
#define NONNAIVE
#endif

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const int IMAX = 9;

template <typename InputIt, typename OutputIt>
OutputIt long_copy(InputIt first, InputIt last, OutputIt d_first) {
  while (first != last)
    *d_first++ = *first++;
  return d_first;
}

template <bool Type, typename In, typename Out> struct CpSel {
  static Out select(In begin, In end, Out out) {
    return long_copy(begin, end, out);
  }
};

template <typename In, typename Out> struct CpSel<true, In, Out> {
  static Out select(In begin, In end, Out out) {
    using in_type = typename std::iterator_traits<In>::value_type;
    auto sz = (end - begin) * sizeof(in_type);
    return static_cast<Out>(memcpy(out, begin, sz));
  }
};

template <typename In, typename Out>
Out nonnaive_copy(In begin, In end, Out out) {
  using in_type = typename std::iterator_traits<In>::value_type;
  using out_type = typename std::iterator_traits<Out>::value_type;
  enum {
    Sel = (std::is_pointer<In>::value && std::is_pointer<Out>::value &&
           std::is_trivially_copyable<in_type>::value &&
           std::is_trivially_copyable<out_type>::value &&
           std::is_same<in_type, out_type>::value)
  };
  return CpSel<Sel, In, Out>::select(begin, end, out);
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
#ifdef NONNAIVE
  nonnaive_copy(arr, arr + nelts, arrcopy2);
#else
  std::copy(arr, arr + nelts, arrcopy2);
  // basically the same for:
  // std::copy_n(arr, nelts, arrcopy2);
#endif
  tfin = high_resolution_clock::now();

#ifdef NAIVE
  std::cout << "non-naive copy: "
#else
  std::cout << "std copy: "
#endif
            << duration_cast<milliseconds>(tfin - tstart).count() << std::endl;

  // sanity: do we have mismatch (we shall not)
  auto mism = std::mismatch(arrcopy, arrcopy + nelts, arrcopy2);
  if (mism.first != arrcopy + nelts || mism.second != arrcopy2 + nelts) {
    std::cout << "mismatch: " << *mism.first << " vs " << *mism.second
              << std::endl;
    std::cout << "at: " << mism.first - arrcopy << std::endl;
  }

  delete[] arrcopy2;
  delete[] arrcopy;
  delete[] arr;
}
