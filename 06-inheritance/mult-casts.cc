//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Static casts with statically calculated offsets
//
//----------------------------------------------------------------------------

#include <iostream>

struct Filler {
  int x, y;
  virtual void filler() = 0;
  virtual ~Filler() {}
};

struct InputFile {
  int b;
  InputFile(int b) : b{b} {}
  virtual ~InputFile() {}
};

struct OutputFile {
  int c;
  OutputFile(int c) : c{c} {}
  virtual ~OutputFile() {}
};

struct IOFile : public Filler, public InputFile, public OutputFile {
  int d;
  void filler() override { std::cout << "  " << std::endl; }
  IOFile(int d) : InputFile(d * 2), OutputFile(d * 3), d{d} {}
};

int main() {
  IOFile *piof = new IOFile{5};
  std::cout << std::hex << "piof = " << piof << ": " << std::dec;
  std::cout << piof->d << std::endl;

  OutputFile *pof = static_cast<OutputFile *>(piof);
  std::cout << std::hex << "pof = " << pof << ": " << std::dec;
  std::cout << pof->c << std::endl;

  InputFile *pif = static_cast<InputFile *>(piof);
  std::cout << std::hex << "pif = " << pif << ": " << std::dec;
  std::cout << pif->b << std::endl;

  piof = static_cast<IOFile *>(pif);
  std::cout << std::hex << "piof = " << piof << ": " << std::dec;
  std::cout << piof->d << std::endl;

  piof = static_cast<IOFile *>(pof);
  std::cout << std::hex << "piof = " << piof << ": " << std::dec;
  std::cout << piof->d << std::endl;
}
