#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

template <typename T> struct reversion_wrapper { T &iterable; };

template <typename T> auto begin(reversion_wrapper<T> w) {
  return std::rbegin(w.iterable);
}

template <typename T> auto end(reversion_wrapper<T> w) {
  return std::rend(w.iterable);
}

template <typename T> reversion_wrapper<T> reverse_cont(T &&iterable) {
  return {iterable};
}

int main() {
  vector<int> v = {2, 3, 5, 7, 11};
  for (auto elt : v)
    cout << elt << endl;
  for (auto elt : reverse_cont(v))
    cout << elt << endl;
}
