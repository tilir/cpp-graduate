#include <iostream>

#include "idomain.hpp"
#include "permloops.hpp"

// permutation loops application:
// (4, 2, 1, 3) applied to [1, 2, 3, 4] yields [3, 1, 4, 2]
constexpr auto test_permloop_apply() {
  using UD4 = UnsignedDomain<1, 4>;
  auto Init = UD4::iota();

  PermLoop<UD4, 4> PL = {4, 2, 1, 3};
  PL.apply(Init.begin(), Init.end());

  return Init;
}

int main() {
  constexpr auto t = test_permloop_apply();
  std::ostream_iterator<int> osit{std::cout, " "};
  std::copy(t.begin(), t.end(), osit);
  std::cout << std::endl;
}