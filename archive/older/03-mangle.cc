struct S {
  int foo(int);
};

extern "C" int foo(int x) { return x; }
