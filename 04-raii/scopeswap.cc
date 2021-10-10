//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of scoped swap
//
//----------------------------------------------------------------------------

#include <iostream>

template <typename T> class scoped_nomove {
  T *ptr_;

public:
  scoped_nomove(T *ptr) : ptr_{ptr} {}
  ~scoped_nomove() { delete ptr_; }
  scoped_nomove(const scoped_nomove &rhs) : ptr_{new T(*rhs.ptr_)} {
    std::cout << "copy ctor\n";
  }
  scoped_nomove &operator=(const scoped_nomove &rhs) {
    std::cout << "copy assign\n";
    if (this == &rhs)
      return *this;
    delete ptr_;
    ptr_ = new T{*rhs.ptr_};
    return *this;
  }
};

template <typename T> class scoped_move {
  T *ptr_;

public:
  scoped_move(T *ptr) : ptr_{ptr} {}
  ~scoped_move() { delete ptr_; }
  scoped_move(const scoped_move &rhs) : ptr_{new T(*rhs.ptr_)} {
    std::cout << "copy ctor\n";
  }
  scoped_move &operator=(const scoped_move &rhs) {
    std::cout << "copy assign\n";
    if (this == &rhs)
      return *this;
    delete ptr_;
    ptr_ = new T{*rhs.ptr_};
    return *this;
  }
  scoped_move(scoped_move &&rhs) : ptr_{rhs.ptr_} {
    std::cout << "move ctor\n";
    rhs.ptr_ = nullptr;
  }
  scoped_move &operator=(scoped_move &&rhs) {
    std::cout << "move assign\n";
    if (this == &rhs)
      return *this;
    std::swap(ptr_, rhs.ptr_);
    return *this;
  }
};

int main() {
  scoped_nomove<int> sn1{new int(10)}, sn2{new int(11)};
  scoped_move<int> sm1{new int(12)}, sm2{new int(13)};

  std::swap(sn1, sn2);
  std::cout << "---\n";
  std::swap(sm1, sm2);
}
