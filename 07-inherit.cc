#include <iostream>

template <typename T>
struct ATempl {
  int x = 1;
};

template <typename T>
struct BTempl : public ATempl<T> {
  int y = 2;
};

using AConcrete = ATempl<int>;
using BConcrete1 = BTempl<int>;
using BConcrete2 = BTempl<double>;

void foo(AConcrete& aref) {
  std::cout << aref.x << std::endl;
}

int main() {
  BConcrete1 b1;
  BConcrete2 b2;

  foo(b1); // OK
  // foo(b2); // FAIL
}

