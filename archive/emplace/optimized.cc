#include <cstring>
#include <iostream>

#include "heavy.hpp"

template <typename T> class Stack {
  struct StackElem {
    StackElem *next;
    T elem;
    StackElem(StackElem *nxt, T e) : next{nxt}, elem{e} {}

    // ctor to emplace something with 1 argument
    template <typename Arg>
    StackElem(StackElem *nxt, Arg &&arg)
        : next{nxt}, elem{std::forward<Arg>(arg)} {}
  };

  struct StackElem *top_;

public:
  Stack() : top_(nullptr) {}
  Stack(const Stack<T> &rhs) = delete;
  ~Stack();
  Stack<T> &operator=(const Stack<T> &rhs) = delete;
  void push_back(const T &);

  // method to emplace something with 1 argument
  template <typename Arg> void emplace_back(Arg &&arg);
};

template <typename T> void Stack<T>::push_back(const T &elem) {
  StackElem *newelem = new StackElem{top_, elem};
  top_ = newelem;
}

template <typename T>
template <typename Arg>
void Stack<T>::emplace_back(Arg &&arg) {
  StackElem *newelem = new StackElem{top_, std::forward<Arg>(arg)};
  // StackElem *newelem = new StackElem{top_, arg};
  top_ = newelem;
}

template <typename T> Stack<T>::~Stack() {
  StackElem *nxt = top_;
  while (nxt != nullptr) {
    StackElem *tmp = nxt->next;
    delete nxt;
    nxt = tmp;
  }
  top_ = nullptr;
}

int main() {
  Stack<Heavy> s;
  for (int i = 0; i != 10; ++i) {
    std::cout << "--- next ---" << std::endl;
    // s.push_back(Heavy(100));
    s.emplace_back(100);
  }
  std::cout << "done" << std::endl;
}
