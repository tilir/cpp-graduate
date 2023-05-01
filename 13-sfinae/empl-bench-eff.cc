//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Example for push vs emplace: push part
// cl /O2 /EHsc empl-bench-eff.cc
//
//-----------------------------------------------------------------------------

#include <cstring>
#include <iostream>

using namespace std;

class Heavy {
  int n;
  int *t;

public:
  explicit Heavy(int sz) : n(sz), t(new int[n]) {
    cout << "Heavy created" << endl;
  }

  // ineffective move
  Heavy(Heavy &&rhs) : Heavy(rhs) {}

  Heavy(const Heavy &rhs) : n(rhs.n), t(new int[n]) {
    cout << "Heavy copied" << endl;
    memcpy(t, rhs.t, n * sizeof(int));
  }

  Heavy &operator=(Heavy &&rhs) = delete;
  Heavy &operator=(const Heavy &rhs) = delete;

  ~Heavy() {
    cout << "Heavy destroyed" << endl;
    delete[] t;
  }
};

template <typename T> class Stack {
  struct StackElem {
    T elem;
    StackElem *next;
    StackElem(StackElem *nxt, T e) : elem(e), next(nxt) {}

    template <typename... Args>
    StackElem(StackElem *nxt, Args &&...args)
        : elem(std::forward<Args>(args)...), next(nxt) {}
  };

  struct StackElem *top_ = nullptr;

public:
  Stack() = default;
  Stack(const Stack<T> &rhs) = delete;
  ~Stack();

  Stack<T> &operator=(const Stack<T> &rhs) = delete;

  void push_back(const T &elem) { top_ = new StackElem(elem, top_); }

  template <typename... Args> void emplace_back(Args &&...args);
};

template <typename T>
template <typename... Args>
void Stack<T>::emplace_back(Args &&...args) {
  top_ = new StackElem(top_, std::forward<Args>(args)...);
}

template <typename T> Stack<T>::~Stack() {
  struct StackElem *nxt = top_;
  while (nxt != nullptr) {
    struct StackElem *tmp = nxt->next;
    delete nxt;
    nxt = tmp;
  }
  top_ = nullptr;
}

int main(void) {
  Stack<Heavy> s;
  for (int i = 0; i != 10; ++i) {
    cout << endl << "next" << endl;
    s.emplace_back(100);
  }
  cout << endl << "we are done\n" << endl;
}
