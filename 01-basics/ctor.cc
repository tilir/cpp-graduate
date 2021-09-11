//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Ctor demo: essence of ctor as function call
//
//-----------------------------------------------------------------------------

#include <iostream>
#include <string>

struct LoudCtor {
  LoudCtor(std::string s) { std::cout << s << " ctor!" << std::endl; }
};

LoudCtor global("Global");

int main() {
  std::cout << "Main started" << std::endl;
  LoudCtor stack("Stack");
  LoudCtor *pheap = new LoudCtor("Heap");
  delete pheap;
}