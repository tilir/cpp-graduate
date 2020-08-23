#pragma once

struct Ptr {
  int x;
  int *p;

  Ptr(int x) : x(x), p(new int[x]) {}
  ~Ptr();
};

void use(Ptr &p);
