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

#include "unnecessary.h"

Ptr::~Ptr() {
  delete[] p;
  x = 0;
  p = nullptr;
}

void use(Ptr &p) {}
