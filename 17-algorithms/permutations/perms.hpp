//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Permutations.
//
// Permutation<T> changes positions of elements from given domain T
//
// Sample permutation in normal form is:
//
// a b c d e f g
// c e f b d g a
//
// it sends a to c, b to e, etc...
//
// Permutation can be expressed in loop form.
// Example from above contains two loops (a c f g) (d b e)
//
// Canonicalization of permutation means:
//   * all single loops are written explicitly
//   * every loop is canonical
//   * loops are sorted by first element in decreasing order
//
// I.e. canonical form of (3 1 6)(5 4) over domain [1, 7) is (4 5)(2)(1 6 3)
//
//------------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <vector>

#include "permcommon.hpp"
#include "permloops.hpp"

//------------------------------------------------------------------------------
//
// Permutation template
//
//------------------------------------------------------------------------------

template <typename T> class Permutation {
  std::vector<PermLoop<T>> loops_;

  // ctors/dtors
public:
  // id permutation over domain
  Permutation();

  // permutation over domain with some initial loops
  template <
      typename RandIt,
      std::enable_if_t<
          std::is_same_v<typename std::iterator_traits<RandIt>::value_type,
                         PermLoop<T>>,
          int> = 0>
  Permutation(RandIt ibeg, RandIt ifin) {
    simplify_loops(ibeg, ifin, back_inserter(loops_));
    sortloops();
#ifdef CHECKS
    check();
#endif
  }

  Permutation(std::initializer_list<PermLoop<T>> ilist)
      : Permutation(ilist.begin(), ilist.end()) {}

  // modifiers
public:
  // left multiply this = lhs * this
  Permutation &lmul(const Permutation &rhs);

  // right multiply this = this * rhs
  Permutation &rmul(const Permutation &rhs);

  // inverted permutation
  void inverse() {
    for (auto &l : loops_)
      l.inverse();
  }

  // selectors
public:
  // apply permutation to elem
  T apply(T elem) const;

  // apply permutation (all loops in direct order) to given table
  template <typename RandIt> void apply(RandIt tbeg, RandIt tend) const {
    for (auto l : loops_)
      l.apply(tbeg, tend);
  }

  // true if permutation contains element
  bool contains(T elem) const;

  // lexicographical less-than
  bool less(const Permutation &rhs) const {
    size_t sz = rhs.loops_.size();
    if (sz != loops_.size())
      return (loops_.size() < sz);
    for (size_t i = 0; i != sz; ++i)
      if (loops_[i] != rhs.loops_[i])
        return (loops_[i] < rhs.loops_[i]);
    return false;
  }

  // true if equals
  bool equals(const Permutation &rhs) const { return rhs.loops_ == loops_; }

  // dump and serialization
public:
  // dump to stream
  std::ostream &dump(std::ostream &buffer) const;

  // service functions
private:
  // sort loops to canonicalize permutation
  void sortloops();

  // enforce invariants
  void check();
};

//------------------------------------------------------------------------------
//
// Standalone operations
//
//------------------------------------------------------------------------------

template <typename T>
bool operator==(const Permutation<T> &lhs, const Permutation<T> &rhs) {
  return lhs.equals(rhs);
}

template <typename T>
bool operator<(const Permutation<T> &lhs, const Permutation<T> &rhs) {
  return lhs.less(rhs);
}

template <typename T>
bool operator!=(const Permutation<T> &lhs, const Permutation<T> &rhs) {
  return !operator==(lhs, rhs);
}

template <typename T>
static inline std::ostream &operator<<(std::ostream &os,
                                       const Permutation<T> &rhs) {
  rhs.dump(os);
  return os;
}

template <typename T>
Permutation<T> product(const Permutation<T> &lhs, const Permutation<T> &rhs) {
  Permutation<T> retval = lhs;
  retval.rmul(rhs);
  return retval;
}

template <typename T> Permutation<T> invert(Permutation<T> lhs) {
  lhs.inverse();
  return lhs;
}

//------------------------------------------------------------------------------
//
// Ctors/dtors
//
//------------------------------------------------------------------------------

template <typename T> Permutation<T>::Permutation() {
  for (auto x = T::start; x <= T::fin; ++x)
    loops_.push_back(PermLoop<T>{x});
  sortloops();
#ifdef CHECKS
  check();
#endif
}

//------------------------------------------------------------------------------
//
// Selectors
//
//------------------------------------------------------------------------------

template <typename T> T Permutation<T>::apply(T elem) const {
  T res = std::move(elem);
  for (auto &ploop : loops_)
    res = ploop.apply(res);
  return res;
}

template <typename T> bool Permutation<T>::contains(T elem) const {
  auto it =
      find_if(loops_.begin(), loops_.end(),
              [elem](const PermLoop<T> &pl) { return pl.contains(elem); });
  return (it != loops_.end());
}

template <typename T>
std::ostream &Permutation<T>::dump(std::ostream &os) const {
  for (auto l : loops_)
    l.dump(os);
  return os;
}

//------------------------------------------------------------------------------
//
// Modifiers
//
//------------------------------------------------------------------------------

template <typename T>
Permutation<T> &Permutation<T>::lmul(const Permutation<T> &input) {
  loops_.insert(loops_.begin(), input.loops_.begin(), input.loops_.end());
  std::vector<PermLoop<T>> outloops;
  simplify_loops(loops_.begin(), loops_.end(), back_inserter(outloops));
  loops_.swap(outloops);
  sortloops();
#ifdef CHECKS
  check();
#endif
  return *this;
}

template <typename T>
Permutation<T> &Permutation<T>::rmul(const Permutation<T> &input) {
  loops_.insert(loops_.end(), input.loops_.begin(), input.loops_.end());
  std::vector<PermLoop<T>> outloops;
  simplify_loops(loops_.begin(), loops_.end(), back_inserter(outloops));
  loops_.swap(outloops);
  sortloops();
#ifdef CHECKS
  check();
#endif
  return *this;
}

//------------------------------------------------------------------------------
//
// Service functions
//
//------------------------------------------------------------------------------

template <typename T> void Permutation<T>::check() {
  if (T::start >= T::fin)
    throw std::runtime_error("Domain error");

  if (loops_.empty())
    throw std::runtime_error("Empty permutation");

  for (auto x = T::start; x <= T::fin; ++x)
    if (!contains(x))
      throw std::runtime_error("Every domain element shall be covered");

  for (auto it = loops_.begin(); it != std::prev(loops_.end()); ++it) {
    if (it->smallest() == std::next(it)->smallest())
      throw std::runtime_error("Loops with equal elements are here");
    if (it->smallest() < std::next(it)->smallest())
      throw std::runtime_error("Canonical form broken");
  }
}

template <typename T> void Permutation<T>::sortloops() {
  std::sort(loops_.begin(), loops_.end(),
            [](const PermLoop<T> &lhs, const PermLoop<T> &rhs) {
              return (lhs.smallest() > rhs.smallest());
            });
}
