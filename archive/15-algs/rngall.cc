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

struct Empty {
  int *begin() { return nullptr; }
  int *end() { return nullptr; }
};

int main() {
  std::string s = "Hello, ranges world";
  std::string_view svv(s);
  std::cout << ti::type_id_with_cvr<decltype(svv)>().pretty_name() << std::endl;
  auto sv = ranges::views::all(s);
  std::cout << ti::type_id_with_cvr<decltype(sv)>().pretty_name() << std::endl;
  auto sagain = sv.base();
  std::cout << ti::type_id_with_cvr<decltype(sagain)>().pretty_name()
            << std::endl;

  Empty e;
  auto ev = ranges::views::all(e);
}
