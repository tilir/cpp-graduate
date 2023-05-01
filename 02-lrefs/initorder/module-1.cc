//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Init order demo: static initialization order
// compile with: see header
//
//-----------------------------------------------------------------------------

#include "module.hh"
#include <iostream>

// two phases:
// 1. static initialization a to 0 before program start
// 2. dynamic initialization a to (b + 1) before main()
int a = b + 1;

int main() { std::cout << "a = " << a << "; b = " << b << std::endl; }
