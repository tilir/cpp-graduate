#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <boost/type_index.hpp>
namespace ti = boost::typeindex;

#include "myranges.hpp"

int main() {
  std::string s = "Hello, ranges world";
  auto sv = ranges::views::all(s);
  auto tk = ranges::views::take(sv, 13);
  std::cout << ti::type_id_with_cvr<decltype(tk)>().pretty_name() << std::endl;

  for (auto &&c : tk)
    std::cout << c;
  std::cout << std::endl;

#if SND
  auto tk2 = ranges::views::take(s, 13);
  std::cout << ti::type_id_with_cvr<decltype(tk2)>().pretty_name() << std::endl;
#endif
}
