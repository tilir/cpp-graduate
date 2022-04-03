//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// orbit in group example
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>

struct Z7 {
  int val;
  Z7(int v = 0) : val(v) {}
  operator int() const { return val; }
  auto operator<=>(const Z7 &) const = default;
};

namespace std {
template <> struct hash<Z7> {
  std::size_t operator()(Z7 K) const {
    std::hash<int> h;
    return h(K.val);
  }
};
} // namespace std

Z7 operator*(Z7 lhs, Z7 rhs) { return {(lhs.val * rhs.val) % 7}; }

template <typename T, typename RandIt>
auto orbit(T num, RandIt gensbeg, RandIt gensend) {
  std::unordered_set<T> orbit;
  std::vector<T> next = {num};
  while (!next.empty()) {
    std::vector<T> tmp{};
    orbit.insert(next.begin(), next.end());
    for (const auto &elem : next)
      for (auto igen = gensbeg; igen != gensend; ++igen)
        if (auto newelem = (*igen) * elem; orbit.count(newelem) == 0) {
          std::cout << elem << " * " << *igen << " = " << newelem << std::endl;
          tmp.push_back(newelem);
        }
    next.swap(tmp);
  }
  return orbit;
}

int main() {
  std::vector gens = {Z7{3}, Z7{5}};
  std::ostream_iterator<int> osit(std::cout, " ");

  for (int n = 1; n < 7; ++n) {
    std::cout << "Starting with " << n << std::endl;
    auto orb = orbit(Z7{n}, gens.begin(), gens.end());
    std::copy(orb.begin(), orb.end(), osit);
    std::cout << std::endl;
  }
}
