//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Example for push vs emplace: push part
// cl /O2 /EHsc empl-bench-ineff.cc
//
//-----------------------------------------------------------------------------

#include <cstring>
#include <iostream>

class Heavy {
  int n;
  int *t;

public:
  Heavy(int sz) : n(sz), t(new int[n]) {
    std::cout << "Heavy created" << std::endl;
  }

  // ineffective move
  Heavy(Heavy &&rhs) : Heavy(rhs) {}

  Heavy(const Heavy &rhs) : n(rhs.n), t(new int[n]) {
    std::cout << "Heavy copied" << std::endl;
    memcpy(t, rhs.t, n * sizeof(int));
  }

  Heavy &operator=(Heavy &&rhs) = delete;
  Heavy &operator=(const Heavy &rhs) = delete;

  ~Heavy() {
    std::cout << "Heavy destroyed" << std::endl;
    delete[] t;
  }
};

template <typename T> class Stack {
  struct StackElem {
    T elem;
    StackElem *next;
    StackElem(T e, StackElem *nxt) : elem(e), next(nxt) {}
  };

  struct StackElem *top_ = nullptr;

public:
  Stack() = default;
  Stack(const Stack<T> &rhs) = delete;
  ~Stack();

  Stack<T> &operator=(const Stack<T> &rhs) = delete;

  void push_back(const T &elem) { top_ = new StackElem(elem, top_); }
};

template <typename T> Stack<T>::~Stack() {
  struct StackElem *nxt = top_;
  while (nxt != nullptr) {
    struct StackElem *tmp = nxt->next;
    delete nxt;
    nxt = tmp;
  }
  top_ = nullptr;
}

int main() {
  Stack<Heavy> s;
  for (int i = 0; i != 5; ++i) {
    std::cout << std::endl << "next" << std::endl;
    s.push_back(Heavy(100));
  }
  std::cout << std::endl << "we are done\n" << std::endl;
}
