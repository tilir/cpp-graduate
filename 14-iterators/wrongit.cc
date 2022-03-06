//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Good vector with bad iterators
// Based on homework of V. Matrenin
// > g++ --std=c++20 wrongit.cc
// > cl /EHsc /std:c++20 wrongit.cc
//
// > g++ -DTRYDIST --std=c++20 wrongit.cc
// > cl /DTRYDIST /EHsc /std:c++20 wrongit.cc
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

namespace containers {

// Exceptions safe dynamical allocated block storage.
template <class T> class VectorStorage {
protected:
  size_t allocated_ = 0;
  size_t size_ = 0;
  T *data_ = nullptr;

  VectorStorage() = default;

  VectorStorage(size_t allocSize)
      : allocated_(allocSize), data_{static_cast<T *>(
                                   ::operator new(sizeof(T) * allocSize))} {}

  VectorStorage(VectorStorage &&toMove) noexcept {
    std::swap(allocated_, toMove.allocated_);
    std::swap(size_, toMove.size_);
    std::swap(data_, toMove.data_);
  }

  VectorStorage &operator=(VectorStorage &&toMove) noexcept {
    std::swap(allocated_, toMove.allocated_);
    std::swap(size_, toMove.size_);
    std::swap(data_, toMove.data_);
    return *this;
  }

  VectorStorage(const VectorStorage &) = delete;
  VectorStorage &operator=(const VectorStorage &) = delete;

  ~VectorStorage() {
    for (size_t i = 0; i < size_; ++i)
      data_[i].~T();
    ::operator delete(data_);
  }
};

// Dynamical array with elements access.
template <class T> class Vector final : private VectorStorage<T> {
  static_assert(std::is_nothrow_move_constructible<T>::value ||
                    std::is_copy_constructible<T>::value,
                "T should be copy constructible or nothrow moveable.");

  using VectorStorage<T>::allocated_;
  using VectorStorage<T>::size_;
  using VectorStorage<T>::data_;

public:
  Vector() = default;
  Vector(Vector &&) = default;
  Vector &operator=(Vector &&) = default;

private:
  // Will be used in few places
  class dummy final {};

  using CopyArg = std::conditional_t<std::is_copy_constructible_v<T>,
                                     const Vector &, dummy>;

  template <class Arg>
  using TCopyCtorEnabled = std::void_t<decltype(Arg(std::declval<Arg &>()))>;
  template <class Arg> using TDefaultCtorEnabled = std::void_t<decltype(Arg())>;

public:
  Vector(CopyArg sd) : VectorStorage<T>{sd.size_} {
    for (; size_ < sd.size_; ++size_)
      new (data_ + size_) T(sd.data_[size_]);
  }
  Vector &operator=(CopyArg sd) {
    Vector copy(sd);
    return *this = std::move(copy);
  }

  template <class Arg = T, class = TCopyCtorEnabled<Arg>>
  Vector(std::initializer_list<T> l) : VectorStorage<T>{l.size()} {
    static_assert(std::is_same_v<Arg, T>, "Arg should be default type (T).");
    for (auto it = l.begin(), end = l.end(); it != end; ++it, ++size_)
      new (data_ + size_) T(*it);
  }
  template <class Arg = T, class = TCopyCtorEnabled<Arg>>
  Vector(size_t size, const T &toCopy) : VectorStorage<T>{size} {
    static_assert(std::is_same_v<Arg, T>, "Arg should be default type (T).");
    for (; size_ < size; ++size_)
      new (data_ + size_) T(toCopy);
  }
  template <class Arg = T, class = TDefaultCtorEnabled<Arg>>
  Vector(size_t size) : VectorStorage<T>{size} {
    static_assert(std::is_same_v<Arg, T>, "Arg should be default type (T).");
    for (; size_ < size; ++size_)
      new (data_ + size_) T{};
  }

  ~Vector() = default;

  static void swap(Vector &ft, Vector &sd) noexcept {
    std::swap(ft.data_, sd.data_);
    std::swap(ft.size_, sd.size_);
    std::swap(ft.allocated_, sd.allocated_);
  }

private:
  template <class Arg>
  using ChooseCopy =
      std::enable_if_t<!std::is_nothrow_move_constructible_v<Arg>, int>;
  template <class Arg>
  using ChooseMove =
      std::enable_if_t<std::is_nothrow_move_constructible_v<Arg>, int>;

  // Sub function for realloc.
  template <class Arg = T, ChooseMove<Arg> = 0>
  void moveOrCopyT(T *dest, T &src) {
    new (dest) T(std::move(src));
  }
  // Sub function for realloc.
  template <class Arg = T, ChooseCopy<Arg> = 0>
  void moveOrCopyT(T *dest, T &src) {
    new (dest) T(src);
  }
  // Ctor for reallocation impl.
  Vector(size_t allocated, dummy) : VectorStorage<T>{allocated} {}

  void realloc(size_t newAllocated) {
    size_t newSize = std::min(newAllocated, size_);
    Vector newVector(newAllocated, dummy{});

    for (; newVector.size_ < newSize; ++newVector.size_)
      moveOrCopyT(newVector.data_ + newVector.size_, data_[newVector.size_]);

    *this = std::move(newVector);
  }

public:
  template <class Arg> void push(Arg &&value) {
    if (size_ == allocated_)
      realloc(2 * allocated_ + 1);

    new (data_ + size_) T{std::forward<Arg>(value)};
    ++size_;
  }
  void pop() {
    if (size_ == 0)
      throw std::invalid_argument("trying pop from empty vector");
    data_[size_ - 1].~T();
    --size_;
  }

  T &back() noexcept { return data_[size_ - 1]; }
  const T &back() const noexcept { return data_[size_ - 1]; }

  T &front() noexcept { return *data_; }
  const T &front() const noexcept { return *data_; }

  class iterator final {
    friend Vector;
    T *ptr_;

    iterator(T *ptr) noexcept : ptr_(ptr) {}

  public:
    iterator(const iterator &) = default;
    iterator &operator=(const iterator &) = default;
    T &operator*() const noexcept { return *ptr_; }
    iterator &operator++() noexcept {
      ++ptr_;
      return *this;
    }
    iterator operator++(int) noexcept {
      iterator copy{*this};
      ++ptr_;
      return copy;
    }
    iterator &operator--() noexcept {
      --ptr_;
      return *this;
    }
    iterator operator--(int) noexcept {
      iterator copy{*this};
      --ptr_;
      return copy;
    }
    bool operator==(const iterator &second) const noexcept = default;
  };

  class const_iterator final {
    iterator it_;

  public:
    const_iterator(const iterator &it) noexcept : it_(it) {}
    const_iterator(const const_iterator &) = default;
    const_iterator &operator=(const const_iterator &) = default;

    const T &operator*() const noexcept { return *it_; }

    const_iterator &operator++() {
      ++it_;
      return *this;
    }
    const_iterator operator++(int) { return it_++; }
    const_iterator &operator--() {
      --it_;
      return *this;
    }
    const_iterator operator--(int) { return it_--; }

    bool operator==(const const_iterator &sd) const noexcept = default;
  };

  iterator begin() noexcept { return iterator(data_); }
  iterator end() noexcept { return iterator(data_ + size_); }
  const_iterator begin() const noexcept { return const_iterator(data_); }
  const_iterator end() const noexcept { return const_iterator(data_ + size_); }
  const_iterator cbegin() const noexcept { return const_iterator(data_); }
  const_iterator cend() const noexcept { return const_iterator(data_ + size_); }

  T &operator[](size_t id) noexcept { return data_[id]; }
  const T &operator[](size_t id) const noexcept { return data_[id]; }

  size_t allocated() const noexcept { return allocated_; }
  size_t size() const noexcept { return size_; }
};

} // namespace containers

int main() {
  containers::Vector<int> v{1, 2, 3};
  for (auto elt : v)
    std::cout << elt << " ";
  std::cout << std::endl;

  std::vector eta{1, 2, 3};
  std::cout << std::distance(eta.begin(), eta.end()) << std::endl;

#ifdef TRYDIST
  std::cout << std::distance(v.begin(), v.end()) << std::endl;
#endif
}
