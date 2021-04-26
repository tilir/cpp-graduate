#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
  std::string s = "hello";

  std::transform(s.begin(), s.end(), s.begin(),
                 [](char c) { return std::toupper(c); });
  std::cout << s << std::endl;
}
