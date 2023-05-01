#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "myres.h"

const int SZ = 10;

int main() {
  std::vector<std::unique_ptr<MyRes>> v(SZ);
  for (int i = 0; i < SZ; ++i)
    v[i] = std::unique_ptr<MyRes>{new MyRes((SZ - i) % 7)};

  std::cout << "Before sort: " << std::endl;

  for (int i = 0; i < SZ; ++i)
    if (v[i].get())
      v[i]->use();

  std::cout << "Sort: " << std::endl;

  std::sort(v.begin(), v.end(), [](auto x, auto y) {
    if (!x.get() || !y.get())
      return false;
    return x->content() < y->content();
  });

  std::cout << "After sort: " << std::endl;
  for (int i = 0; i < SZ; ++i)
    if (v[i].get())
      v[i]->use();
}
