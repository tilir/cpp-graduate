//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Integral domains.
//
// Integral domain encodes integral diapasone [A, B]
// Say Idom<unsigned, 1, 7> encodes range [1, 2, 3, 4, 5, 6, 7]
// Since we want to use it as a key in unordered maps and sets it also
// defines all required boilerplate.
//
//------------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T, T start_, T fin_> class Idom {
  T val_;

public:
  constexpr Idom(T val = start_) : val_(val) {
    if (val > fin_)
      throw std::out_of_range(std::string("value too big for domain: ") +
                              std::to_string(val));
    if (val < start_)
      throw std::out_of_range("value too small for domain");
  }

  constexpr operator T() const { return val_; }
  constexpr bool operator==(const Idom &) const = default;

  static constexpr T start = start_;
  static constexpr T fin = fin_;

  // returns basic permutation for this domain from start to fin
  static constexpr auto iota() {
    std::array<Idom, fin - start + 1> ret;
    std::iota(ret.begin(), ret.end(), start);
    return ret;
  }

  std::ostream &dump(std::ostream &os) {
    os << val_;
    return os;
  }
};

template <typename T, T start_, T fin_>
inline std::ostream &operator<<(std::ostream &os, Idom<T, start_, fin_> elt) {
  elt.dump(os);
  return os;
}

namespace std {
template <typename T, T start_, T fin_> struct hash<Idom<T, start_, fin_>> {
  std::size_t operator()(const Idom<T, start_, fin_> &K) const {
    std::hash<T> h;
    return h(K);
  }
};
} // namespace std

template <unsigned start_, unsigned fin_>
using UnsignedDomain = Idom<unsigned, start_, fin_>;

template <char start_, char fin_> using CharDomain = Idom<char, start_, fin_>;