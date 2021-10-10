#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

template <typename InputIt, typename OutputIt, typename UnaryPred,
          typename UnaryFunc>
OutputIt transform_copy_if(InputIt first, InputIt last, OutputIt d_first,
                           UnaryPred pred, UnaryFunc func) {
  for (; first != last; ++first) {
    if (pred(*first))
      *d_first = func(*first);
    d_first++;
  }
  return d_first;
}

int main() {
  std::string s = "1 9 2 4 8 6 3 5 7";
  std::istringstream is{s};
  auto &os = std::cout;

  std::istream_iterator<int> start{is}, fin{};
  std::ostream_iterator<int> d_start{std::cout, " "};

  transform_copy_if(
      start, fin, d_start, [](int n) { return n < 5; },
      [](int n) { return n * 2; });
  std::cout << std::endl;
}
