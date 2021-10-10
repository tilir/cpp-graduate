#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

template <typename C, typename It>
auto remove_constness_reverse_meyers(C &v, It cvi) {
  static_assert(std::is_same<typename C::const_reverse_iterator, It>::value,
                "Iterator shall be from container and reverse");
  auto rsi = v.rbegin();
  std::advance(rsi, std::distance<decltype(cvi)>(rsi, cvi));
  return rsi;
}

template <typename C, typename It> auto remove_constness_meyers(C &v, It cvi) {
  static_assert(std::is_same<typename C::const_iterator, It>::value,
                "Iterator shall be from container");
  auto si = v.begin();
  std::advance(si, std::distance<decltype(cvi)>(si, cvi));
  return si;
}

template <typename C> void test() {
  C cont{1, 2, 3, 4, 5, 6, 7};
  auto crvi = cont.crbegin();

  // 1 2 3 4 5 6 7 <

  std::advance(crvi, 4);

  // 1 2 3 < 4 5 6 7

  auto cvi = crvi.base();

  // 1 2 3 > 4 5 6 7

  auto vi = remove_constness_meyers(cont, cvi);
  *vi *= 3;
  auto rvi = remove_constness_reverse_meyers(cont, crvi);
  *rvi *= 2;

  // 1 2 6 <- * -> 12 5 6 7

  if ((6 == *crvi) && (12 == *vi))
    std::cout << "#" << typeid(C).name() << " PASSED\n" << std::endl;
  else
    std::cout << "#" << typeid(C).name() << " FAILED\n" << std::endl;
}

int main() {
  test<std::list<int>>();
  test<std::vector<int>>();
  test<std::deque<int>>();
}
