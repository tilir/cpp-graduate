#include <iostream>
#include <locale>
#include <string>
#include <vector>

struct my_ctype : public std::ctype<char> {
  static const mask *make_table() {
    static std::vector<mask> v(classic_table(), classic_table() + table_size);
    v[':'] |= space;
    return &v[0];
  }

  my_ctype(size_t refs = 0) : ctype(make_table(), false, refs) {}
};

int main() {
  std::string s1, s2, s3, s4;
  std::locale x(std::locale::classic(), new my_ctype);
  std::cin.imbue(x);
  std::cin >> s1 >> s2 >> s3 >> s4;
  std::cout << s1 << std::endl
            << s2 << std::endl
            << s3 << std::endl
            << s4 << std::endl;
}
