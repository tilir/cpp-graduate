//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of unique_ptr sizes
//
//----------------------------------------------------------------------------

#include <iostream>
#include <memory>

enum { SZ = 1000 };

struct CDeleterTy {
  void operator()(int *t) { delete[] t; }
};

auto LDeleter = [](int *t) { delete[] t; };
using LDeleterTy = decltype(LDeleter);

void FDeleter(int *t) { delete[] t; }
using FDeleterTy = decltype(&FDeleter);

int main() {
  int *Uip = new int[SZ]();
  std::unique_ptr<int, CDeleterTy> Uic{new int[SZ]()};
  std::unique_ptr<int, LDeleterTy> Uil{new int[SZ](), LDeleter};
  std::unique_ptr<int, FDeleterTy> Uif{new int[SZ](), FDeleter};

  std::cout << "pi:" << sizeof(Uip) << "\tuic:" << sizeof(Uic);
  std::cout << "\tuil:" << sizeof(Uil) << "\tuif:" << sizeof(Uif) << std::endl;

  delete[] Uip;
}
