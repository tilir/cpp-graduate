#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "myranges.hpp"

int main() {
  std::string s = "1 9 2 4 8 6 3 5 7";
  std::istringstream is{s};

  auto v = ranges::istream_view<int>(is) |
           ranges::views::filter([](int n) { return n < 5; }) |
           ranges::views::transform([](int n) { return n * 2; });
  ranges::copy(v, ranges::ostream_iterator<int>{std::cout, " "});
  std::cout << std::endl;
}
