//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Third attempt: rather good implementation
//  try: g++ myvec-3.cc  -O0 -g
//  for both: valgrind ./a.out
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "controllable.hh"

int Controllable::control = 5;

template <typename T> void construct(T *p, const T &rhs) { new (p) T(rhs); }

template <class T> void destroy(T *p) noexcept { p->~T(); }

template <typename FwdIter> void destroy(FwdIter first, FwdIter last) noexcept {
  while (first != last)
    destroy(&*first++);
}

template <typename T> struct MyVectorBuf {
protected:
  T *arr_;
  size_t size_, used_ = 0;

protected:
  MyVectorBuf(const MyVectorBuf &) = delete;
  MyVectorBuf &operator=(const MyVectorBuf &) = delete;
  MyVectorBuf(MyVectorBuf &&rhs) noexcept
      : arr_(rhs.arr_), size_(rhs.size_), used_(rhs.used_) {
    rhs.arr_ = nullptr;
    rhs.size_ = 0;
    rhs.used_ = 0;
  }

  MyVectorBuf &operator=(MyVectorBuf &&rhs) noexcept {
    std::swap(arr_, rhs.arr_);
    std::swap(size_, rhs.size_);
    std::swap(used_, rhs.used_);
    return *this;
  }

  MyVectorBuf(size_t sz = 0)
      : arr_((sz == 0) ? nullptr
                       : static_cast<T *>(::operator new(sizeof(T) * sz))),
        size_(sz) {}

  ~MyVectorBuf() {
    destroy(arr_, arr_ + used_);
    ::operator delete(arr_);
  }
};

template <typename T> struct MyVector : private MyVectorBuf<T> {
  using MyVectorBuf<T>::arr_;
  using MyVectorBuf<T>::size_;
  using MyVectorBuf<T>::used_;

  explicit MyVector(size_t sz = 0) : MyVectorBuf<T>(sz) {}

  MyVector(MyVector &&rhs) = default;

  MyVector &operator=(MyVector &&rhs) = default;

  MyVector(const MyVector &rhs) : MyVectorBuf<T>(rhs.used_) {
    while (used_ < rhs.used_) {
      construct(arr_ + used_, rhs.arr_[used_]);
      used_ += 1;
    }
  }

  MyVector &operator=(const MyVector &rhs) {
    MyVector tmp(rhs);
    std::swap(*this, tmp);
    return *this;
  }

  T top() const {
    if (used_ < 1)
      throw std::runtime_error("Vector is empty");
    return arr_[used_ - 1];
  }

  void pop() {
    if (used_ < 1)
      throw std::runtime_error("Vector is empty");
    used_ -= 1;
    destroy(arr_ + used_);
  }

  void push(const T &t) {
    assert(used_ <= size_);
    if (used_ == size_) {
      MyVector tmp(size_ * 2 + 1);
      while (tmp.used_ < used_)
        tmp.push(arr_[tmp.used_]);
      tmp.push(t);
      std::swap(*this, tmp);
      return;
    }
    construct(arr_ + used_, t);
    used_ += 1;
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
