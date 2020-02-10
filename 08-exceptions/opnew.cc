#include <cstdlib>
#include <iostream>
#include <list>

void *operator new(std::size_t n) {

  // BAD idea. Why?
  // std::cout << "Alloc:" << n << "\n";

  void *p = malloc(n);
  printf("Alloc: %p, size is %zu\n", p, n);
  return p;
}

void operator delete(void *mem) noexcept {
  printf("Free: %p\n", mem);
  free(mem);
}

int main() {
  std::list<int> l;
  l.push_back(42);

  // Lets test: how do you think?
#ifdef COUT
  std::cout << "Hello " << l.size() << "\n";
#endif
}
