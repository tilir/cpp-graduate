//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example: wrong pairing of new/delete and consequences
//  compile with: g++ -g newdelete.cc
//  try x/20x P-1 in gdb
//
//----------------------------------------------------------------------------

#include <iostream>

struct MySmallClass {
  int t = 1;
  MySmallClass() { std::cout << "small ctor" << std::endl; }
  ~MySmallClass() { std::cout << "small dtor" << std::endl; }
};

struct MyBigClass {
  int t = 1, p = 2, q = 3;
  MyBigClass() { std::cout << "big ctor" << std::endl; }
  ~MyBigClass() { std::cout << "big dtor" << std::endl; }
};

int main() {
  MyBigClass *S = new MyBigClass;
  MySmallClass *T = new MySmallClass;
  MyBigClass *P = new MyBigClass[5];
  MySmallClass *Q = new MySmallClass[7];
  delete[] T; // terribly wrong
}