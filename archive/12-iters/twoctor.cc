#include <cassert>
#include <iostream>
#include <iterator>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

template <typename T> class MyVector {
  size_t size_, capacity_;
  T *arr_;

public:
  MyVector(size_t nelts, T value)
      : size_(nelts), capacity_(size_), arr_(new T[nelts]) {
    for (auto n = 0; n != nelts; ++n)
      arr_[n] = value;
  }
  template <typename Iter>
  MyVector(Iter fst, Iter lst)
      : size_(std::distance(fst, lst)), capacity_(size_), arr_(new T[size_]) {
    int *p = arr_;

    // assume no exceptions, etc, just for simplicity
    for (auto it = fst; it != lst; ++it)
      *p++ = *it;
  }
  ~MyVector() { delete[] arr_; }

public:
  int *begin() { return arr_; }
  int *end() { return arr_ + size_; }
};

int main() {
  vector<int> svec(2, 2);
#ifdef BUG
  MyVector<int> mvec(2, 2);
#endif
}
