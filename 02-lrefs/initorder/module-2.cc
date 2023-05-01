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

// two phases:
// 1. static initialization b to 0 before program start
// 2. dynamic initialization b to (a + 1) before main()
int b = a + 1;
