//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Unnecessary dtor code demo
// compile with: g++ -S -O1 -g0 -masm=intel unnecessary-2.cc
//
//-----------------------------------------------------------------------------

#pragma once

struct Ptr {
  int x;
  int *p;

  Ptr(int x) : x(x), p(new int[x]) {}
  ~Ptr();
};

void use(Ptr &p);
