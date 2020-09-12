struct S {
  int foo(int);
};

int S::foo(int x) { return x; }

int foo(S *p, int x) { return p->foo(x); }

extern "C" int foo(int x) { return x; }
