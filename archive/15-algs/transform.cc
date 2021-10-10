#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
  std::string s = "hello";

  std::transform(s.begin(), s.end(), s.begin(), std::toupper);
  std::cout << s << std::endl;
}
