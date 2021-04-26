#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

int main() {
  std::string s = "1 9 2 4 8 6 3 5 7";
  std::istringstream is{s};
  auto &os = std::cout;

  std::istream_iterator<int> start{is}, fin{};

  std::for_each(start, fin, [&os](int d) {
    if (d < 5)
      os << d * 2 << " ";
  });
  std::cout << std::endl;
}
