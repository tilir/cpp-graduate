#include <cstring>
#include <iostream>

#include "heavy.hpp"

template <typename T> class Stack {
  struct StackElem {
    T elem;
    StackElem *next;
    StackElem(StackElem *nxt, T e) : next{nxt}, elem{e} {}

    template <typename ... Args>
    StackElem(StackElem *nxt, Args &&... args)
        : next{nxt}, elem{std::forward<Args>(args)...} {}
  };

  struct StackElem *top_;

public:
  Stack() : top_(nullptr) {}
  Stack(const Stack<T> &rhs) = delete;
  ~Stack();

  Stack<T> &operator=(const Stack<T> &rhs) = delete;

  void push_back(const T &);

  template <typename ... Args> void emplace_back(Args &&... args);
};

template <typename T> void Stack<T>::push_back(const T &elem) {
  StackElem *newelem = new StackElem{top_, elem};
  top_ = newelem;
}

template <typename T>
template <typename... Args>
void Stack<T>::emplace_back(Args &&... args) {
  StackElem *newelem = new StackElem{top_, std::forward<Args>(args)...};
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
