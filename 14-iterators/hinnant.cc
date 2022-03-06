//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Hinnant trick
//
//-----------------------------------------------------------------------------

#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

template <typename Container, typename ConstIterator>
typename Container::iterator remove_constness(Container &c, ConstIterator it) {
  return c.erase(it, it);
}

template <typename C> void test() {
  C cont{1, 2, 3, 4, 5, 6, 7};
  auto crvi = cont.crbegin();

  // 1 2 3 4 5 6 7 <

  std::advance(crvi, 4);

  // 1 2 3 < 4 5 6 7

  auto cvi = crvi.base();

  // 1 2 3 > 4 5 6 7

  auto vi = remove_constness(cont, cvi);
  *vi *= 3;

  // not working for reverse iterators
  // auto rvi = remove_constness(cont, crvi);
  // *rvi *= 2;

  // 1 2 6 <- * -> 12 5 6 7

  if (/* (6 == *crvi) && */ (12 == *cvi))
    std::cout << "#" << typeid(C).name() << " PASSED\n" << std::endl;
  else
    std::cout << "#" << typeid(C).name() << " FAILED\n" << std::endl;
}

int main() {
  test<std::list<int>>();
  test<std::vector<int>>();
  test<std::deque<int>>();
}
