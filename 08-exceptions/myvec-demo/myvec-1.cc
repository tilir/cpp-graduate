//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  First naive implementation: not exception safe
//  try: g++ myvec-1.cc  -O0 -g -DEXTEND_CONTROL
//  try: g++ myvec-1.cc  -O0 -g
//  for both: valgrind ./a.out
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "controllable.hh"

int Controllable::control = 5;

template <typename T> class MyVector {
  T *arr_ = nullptr;
  size_t size_, used_ = 0;

public:
  explicit MyVector(size_t sz = 0) : arr_(new T[sz]), size_(sz) {}

  MyVector(const MyVector &rhs)
      : arr_(new T[rhs.size_]), size_(rhs.size_), used_(rhs.used_) {
    for (size_t idx = 0; idx != size_; ++idx)
      arr_[idx] = rhs.arr_[idx];
  }

  MyVector(MyVector &&rhs) noexcept
      : arr_(rhs.arr_), size_(rhs.size_), used_(rhs.used_) {
    rhs.arr_ = nullptr;
    rhs.size_ = 0;
    rhs.used_ = 0;
  }

  MyVector &operator=(MyVector &&rhs) noexcept {
    std::swap(arr_, rhs.arr_);
    std::swap(size_, rhs.size_);
    std::swap(used_, rhs.used_);
  }

  MyVector &operator=(const MyVector &rhs) {
    if (this != &rhs) {
      size_ = rhs.size_;
      used_ = rhs.used_;
      delete[] arr_;
      arr_ = new T[size_];
      for (size_t idx = 0; idx != size_; ++idx)
        arr_[idx] = rhs.arr_[idx];
    }
    return *this;
  }

  ~MyVector() { delete[] arr_; }

  T pop() {
    if (used_ < 1)
      throw std::runtime_error("Vector is empty");
    used_ -= 1;
    return arr_[used_];
  }

  void push(const T &t) {
    assert(used_ <= size_);
    if (used_ == size_) {
      std::cout << "Realloc\n";
      size_t newsz = size_ * 2 + 1;
      T *newarr = new T[newsz];
      for (size_t idx = 0; idx != size_; ++idx)
        newarr[idx] = arr_[idx];
      delete[] arr_;
      arr_ = newarr;
      size_ = newsz;
      assert(used_ < size_);
    }
    arr_[used_] = t;
    ++used_;
  }

  size_t size() const { return used_; }
  size_t capacity() const { return size_; }
};

void test1() {
  Controllable c1, c2, c3;
  MyVector<Controllable> vv1(1);
  vv1.push(c1);
  vv1.push(c2);
  vv1.push(c3);
  std::cout << "Invoke copy ctor\n";
  MyVector<Controllable> vv2(vv1); // oops
  std::cout << vv2.size() << std::endl;
}

int main() {
  try {
    test1();
  } catch (std::bad_alloc &) {
    std::cout << "Exception catched\n";
  }
}
