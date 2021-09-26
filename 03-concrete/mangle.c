//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of mangling for C
//  compile with: g++ -g0 -O1 -masm=intel -S mangle.c
//
//----------------------------------------------------------------------------

extern int myfunc(const char *format, ...);

int foo(int x) {
  myfunc("%d\n", x); // will become myfunc@PLT
  return x;
}
