//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of mangling for C++
//  compile with: g++ -g0 -O1 -masm=intel -S mangle.cc
//
//----------------------------------------------------------------------------

struct S {
  int foo(int) __attribute__((noinline));
};

int foo(S *p, int x) { return p->foo(x); } // _Z3fooP1Si

int foo(int x) { return x; } // _Z3fooi

int S::foo(int x) { return x; } // _ZN1S3fooEi

extern "C" int bar(int x) { return x; } // bar (can not overload foo!)
