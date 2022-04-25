//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Permutation loops.
//
// Permutation loop like (a c d) encodes permutation
//
// a b c d e
// c b d a e
//
// i.e. fixes umentioned points and permutes a --> c --> d --> a
//
// Loops (a c d), (d a c) and (c d a) are equivalent. First is normal, i.e.
// it has smallest element first and serves as class representative
//
// Main assumptions are:
// * loop is non-empty
// * all elements in loop are unique
// * first element is smallest one
//
// use -DCHECKS to build with expensive runtime checks of this assumptions on
// creation and on any modification
//
//------------------------------------------------------------------------------

#ifndef PERMLOOPS_GUARD_
#define PERMLOOPS_GUARD_

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_set>

//------------------------------------------------------------------------------
//
// Permloop template
//
//------------------------------------------------------------------------------

template <typename T, size_t N, size_t... Ns>
constexpr std::array<T, N> make_array_impl(std::initializer_list<T> t,
                                           std::index_sequence<Ns...>) {
  return std::array<T, N>{*(t.begin() + Ns)...};
}

template <typename T, size_t N>
constexpr std::array<T, N> make_array(std::initializer_list<T> t) {
  return make_array_impl<T, N>(t, std::make_index_sequence<N>());
}

template <typename T, size_t N> class PermLoop {
  std::array<T, N> loop_;

  // traits
public:
  using value_type = T;

  // ctors/dtors
public:
#if 0
  // from begin-end range (vector, list, etc)
  template <typename FwdIter>
  constexpr PermLoop(FwdIter b, FwdIter e) : loop_(b, e) {
    reroll();
    check();
  }
#endif

  // from initializer list
  constexpr PermLoop(std::initializer_list<T> ls)
      : loop_(make_array<T, N>(ls)) {
    reroll();
    check();
  }

  // Modifiers
public:
#if 0
  // add element to permutation loop
  constexpr void add(T x) {
    loop_.push_back(x);
    reroll();
    check();
  }
#endif

  // inverse permutation loop:
  // (a b c) to (a c b), etc
  constexpr void inverse() {
    if (loop_.size() < 3)
      return;
    std::reverse(std::next(loop_.begin()), loop_.end());
    check();
  }

  // Getters
public:
  // get smallest element in loop
  constexpr T smallest() const { return loop_.front(); }

  // return true if loop contains given element
  constexpr bool contains(T x) const {
    return std::find(loop_.begin(), loop_.end(), x) != loop_.end();
  }

  // apply loop to given element
  constexpr T apply(T x) const;

  // permute on table with loop
  // this is more effective then element-wise application
  template <typename RandIt>
  constexpr void apply(RandIt tbeg, RandIt tend) const;

  // true if loops are equal
  constexpr bool equals(const PermLoop &rhs) const {
    return loop_ == rhs.loop_;
  }

  // default comparisons
  constexpr auto operator<=>(const PermLoop &rhs) const = default;

  // lexicographical less-than
  constexpr bool less(const PermLoop &rhs) const {
    size_t sz = rhs.loop_.size();
    if (loop_.size() != sz)
      return loop_.size() < sz;
    for (size_t i = 0; i != sz; ++i)
      if (loop_[i] != rhs.loop_[i])
        return loop_[i] < rhs.loop_[i];
    return false;
  }

  // size of loop
  constexpr size_t size() const { return loop_.size(); }

  // Serialization and dumps
public:
  // dump to given stream
  std::ostream &dump(std::ostream &buffer) const;

  // return as string
  std::string to_string() const {
    std::ostringstream buffer;
    dump(buffer);
    return buffer.str();
  }

  // return as vector
  constexpr std::vector<T> to_vector() const { return loop_; }

  // Service functions
private:
  // CHECK postcondition consistency after modification
  constexpr void check();

  // roll to canonical: smallest element first
  constexpr void reroll() {
    auto smallest = std::min_element(loop_.begin(), loop_.end());
    std::rotate(loop_.begin(), smallest, loop_.end());
  }
};

//------------------------------------------------------------------------------
//
// Standalone operations
//
//------------------------------------------------------------------------------

template <typename T, size_t N>
std::ostream &operator<<(std::ostream &os, const PermLoop<T, N> &rhs) {
  rhs.dump(os);
  return os;
}

// creates array of loops from permutation given by table
// say: [d, c, e, g, b, f, a] with type CharDomain<a, g>
// gives: [(a, d, g), (b, c, e), (f)]
template <typename T, typename RandIt, typename OutIt>
constexpr void create_loops(RandIt tbeg, RandIt tend, OutIt lbeg);

// products all input loops over [start, fin) to minimize them
// for example:
// (a, c, f, g) (b, c, d) (a, e, d) (f, a, d, e) (b, g, f, a, e)
// simplifies to:
// (a, d, g) (b, c, e) (f)
// see TAOCP, Alg. 1.3.3B
template <typename RandIt, typename OutIt>
constexpr void simplify_loops(RandIt tbeg, RandIt tend, OutIt lbeg);

//------------------------------------------------------------------------------
//
// Selectors
//
//------------------------------------------------------------------------------

template <typename T, size_t N> constexpr T PermLoop<T, N>::apply(T x) const {
  auto it = std::find(loop_.begin(), loop_.end(), x);
  if (it == loop_.end())
    return x;
  auto nxt = std::next(it);
  if (nxt == loop_.end())
    return *loop_.begin();
  return *nxt;
}

template <typename T, size_t N>
template <typename RandIt>
constexpr void PermLoop<T, N>::apply(RandIt tbeg, RandIt tend) const {
  static_assert(T::start < T::fin);
#if 0
  assert(loop_.front() >= T::start);
  assert(loop_.back() <= T::fin);
  assert(tend - tbeg == T::fin - T::start + 1);
#endif
  size_t nxt = loop_.front() - T::start;
  T tmp = tbeg[nxt];
  for (auto l : loop_) {
    size_t prev = nxt;
    nxt = l - T::start;
    if (l == loop_.front())
      continue;
    tbeg[prev] = tbeg[nxt];
  }
  tbeg[nxt] = tmp;
}

//------------------------------------------------------------------------------
//
// Dump and printing
//
//------------------------------------------------------------------------------

template <typename T, size_t N>
std::ostream &PermLoop<T, N>::dump(std::ostream &os) const {
  for (const auto &t : loop_) {
    if (t == loop_.front())
      os << "(";
    os << t;
    if (t == loop_.back())
      os << ")";
    else
      os << " ";
  }
  return os;
}

//------------------------------------------------------------------------------
//
// Service functions
//
//------------------------------------------------------------------------------

template <typename T, size_t N> constexpr void PermLoop<T, N>::check() {
  // check any elements
  if (loop_.empty())
    throw std::runtime_error("PermLoop shall be non-empty");

#ifdef CHECKS
  // check unique elements
  std::unordered_set<T> uniqs(loop_.begin(), loop_.end());
  if (uniqs.size() != loop_.size())
    throw std::runtime_error("PermLoop elements shall be unique");

  // check minimal element is first
  auto smallest = std::min_element(loop_.begin(), loop_.end());
  if (*smallest != loop_.front())
    throw std::runtime_error("Unnormalized state detected");
#endif
}

//------------------------------------------------------------------------------
//
// Standalone operations
//
//------------------------------------------------------------------------------

template <typename RandIt, typename OutIt>
constexpr void create_loops(RandIt tbeg, RandIt tend, OutIt lbeg) {
  using T = typename std::decay<decltype(*tbeg)>::type;
  using OutT = typename OutIt::container_type::value_type;
  std::vector<bool> marked(tend - tbeg, false);

  for (auto mit = marked.begin(); mit != marked.end();
       mit = std::find(mit + 1, marked.end(), false)) {
    auto relt = mit - marked.begin();
    auto t = static_cast<T>(T::start + relt);
    OutT perm = {t};
    marked[relt] = true;
    for (T nxt = tbeg[relt]; nxt != t; nxt = tbeg[nxt - T::start]) {
      perm.add(nxt);
      marked[nxt - T::start] = true;
    }
    *lbeg = perm;
    lbeg++;
  }
}

template <typename RandIt, typename OutIt>
constexpr void simplify_loops(RandIt tbeg, RandIt tend, OutIt lbeg) {
  using T = typename std::decay<decltype(*tbeg)>::type::value_type;
  std::vector<T> table(T::fin - T::start + 1, T::start);
  std::iota(table.begin(), table.end(), T::start);
  for (auto loopit = std::make_reverse_iterator(tend);
       loopit != std::make_reverse_iterator(tbeg); ++loopit)
    loopit->apply(table.begin(), table.end());
  create_loops(table.begin(), table.end(), lbeg);
}

#endif
