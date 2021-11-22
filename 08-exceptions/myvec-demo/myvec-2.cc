#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>

using std::cout;
using std::endl;
using std::runtime_error;
using std::swap;

template <typename T>
T *safe_copy(const T *src, size_t srcsize, size_t dstsize) {
  assert(srcsize <= dstsize);
  T *dest = new T[dstsize];
  try {
    for (size_t idx = 0; idx != srcsize; ++idx)
      dest[idx] = src[idx];
  } catch (...) {
    delete[] dest;
    throw;
  }
  return dest;
}

template <typename T> class MyVector {
  T *arr_ = nullptr;
  size_t size_, used_;

public:
  MyVector(size_t sz) : arr_(new T[sz]), size_(sz), used_(0) {}

  MyVector(const MyVector &rhs) {
    arr_ = safe_copy(rhs.arr_, rhs.size_, rhs.size_);
    size_ = rhs.size_;
    used_ = rhs.used_;
  }

  MyVector(MyVector &&rhs)
      : arr_(rhs.arr_), size_(rhs.size_), used_(rhs.used_) {
    rhs.arr_ = nullptr;
    rhs.size_ = 0;
    rhs.used_ = 0;
  }

  MyVector &operator=(MyVector &&rhs) {
    swap(arr_, rhs.arr_);
    swap(size_, rhs.size_);
    swap(used_, rhs.used_);
  }

  MyVector &operator=(const MyVector &rhs) {
    MyVector tmp(rhs); // конструктор копирования
    swap(*this, tmp);  // move-ctor, move-assign
    return *this;
  }

  T top() const {
    if (used_ < 1)
      throw runtime_error("Vector is empty");
    return arr_[used_ - 1];
  }

  void pop() {
    if (used_ < 1)
      throw runtime_error("Vector is empty");
    used_ -= 1;
  }

  void push(const T &t) {
    // assert (used_ <= size_);
    if (used_ == size_) {
      size_t newsz = size_ * 2 + 1;
      T *newarr = safe_copy(arr_, size_, newsz);
      delete[] arr_;
      arr_ = newarr;
      size_ = newsz;
      // assert (used_ < size_);
    }

    arr_[used_] = t;
    ++used_;
  }

  size_t size() const { return used_; }
  size_t capacity() const { return size_; }
};

class RefBind {
  static int g;
  int &ref;

public:
  RefBind() : ref(g) {}
  RefBind(int x) : ref(x) {}
  int get() const { return ref; }
};

int RefBind::g = 0;

int main() {
  MyVector<int> v(10);
  for (int i = 0; i < 30; ++i)
    v.push(i);
  MyVector<int> v2(v);
  MyVector<int> v3(10);
  for (int i = 0; i < 5; ++i)
    v.pop();
  v3 = v;
  for (int i = 0; i < 5; ++i)
    v.pop();

  cout << v.size() << endl;
  cout << v2.size() << endl;
  cout << v3.size() << endl;

#if VECVEC
  MyVector<MyVector<int>> vv(1);
  vv.push(v);
  vv.push(v2);
  vv.push(v3);
  cout << vv.size() << endl;
#endif

#if VECREF
  MyVector<RefBind> vr(10);
  vr.push(10);
  MyVector<RefBind> vr2(vr);
#endif
}
