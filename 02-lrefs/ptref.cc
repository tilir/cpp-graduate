//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example: function signature for pointer and reference
//  compile with: g++ -O1 -g0 -masm=intel -S ptref.cc
//
//----------------------------------------------------------------------------

int g;

void foo(int *x) { g = *x; }

void bar(int &x) { g = x; }
