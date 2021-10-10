#include "04-unnecessary.h"

Ptr::~Ptr() {
  delete[] p;
  x = 0;
  p = nullptr;
}

void use(Ptr &p) {}
