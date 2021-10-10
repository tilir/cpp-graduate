#include <iostream>
#include <utility>

using std::boolalpha;
using std::cout;
using std::endl;
using std::false_type;
using std::true_type;
using std::void_t;

template <typename, typename = void_t<>>
struct has_typedef_foobar : false_type {};

template <typename T>
struct has_typedef_foobar<T, void_t<typename T::foobar>> : true_type {};

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
