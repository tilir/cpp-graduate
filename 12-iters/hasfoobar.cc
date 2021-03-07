#include <iostream>

using std::boolalpha;
using std::cout;
using std::endl;

template <typename T> struct has_typedef_foobar {
  typedef char yes[1];
  typedef char no[2];

  template <typename C> static yes &test(typename C::foobar *);
  template <typename> static no &test(...);

  static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

struct foo {
  typedef float foobar;
};

struct bar {};

struct buz {
  float foobar();
};

int main() {
  cout << boolalpha << has_typedef_foobar<foo>::value << " "
       << has_typedef_foobar<bar>::value << " "
       << has_typedef_foobar<buz>::value << " " << endl;
}
