#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

int main() {
  std::string s = "1 9 2 4 8 6 3 5 7";
  std::istringstream is{s};

  std::vector<int> v;
  std::istream_iterator<int> start{is}, fin{};
  std::ostream_iterator<int> d_start{std::cout, " "};

  std::copy_if(start, fin, std::back_inserter(v), [](int n) { return n < 5; });
  std::transform(v.begin(), v.end(), d_start, [](int n) { return n * 2; });
  std::cout << std::endl;
}
