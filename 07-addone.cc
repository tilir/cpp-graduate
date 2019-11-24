#include <iostream>
#include <locale>

namespace {

int geti() {
  static int i = std::ios_base::xalloc();
  return i;
}

std::ostream &add_one(std::ostream &os) {
  os.iword(geti()) = 1;
  return os;
}

std::ostream &add_none(std::ostream &os) {
  os.iword(geti()) = 0;
  return os;
}

} // namespace

struct my_num_put : std::num_put<char> {
  iter_type do_put(iter_type s, std::ios_base &f, char_type fill,
                   long v) const {
    return num_put<char>::do_put(s, f, fill, v + f.iword(geti()));
  }
  iter_type do_put(iter_type s, std::ios_base &f, char_type fill,
                   unsigned long v) const {
    return num_put<char>::do_put(s, f, fill, v + f.iword(geti()));
  }
};

int main() {
  std::cout.imbue(std::locale(std::locale(), new my_num_put));
  std::cout << add_one << 10 << " " << 11 << std::endl
            << add_none << 10 << " " << 11 << std::endl;
}
