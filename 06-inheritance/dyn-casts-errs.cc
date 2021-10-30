//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Dynamic cast errors
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <stdexcept>

struct File {
  int a;
  File(int a) : a{a} { std::cout << "File ctor" << std::endl; }
  virtual ~File() {}
};

struct InputFile : virtual public File {
  int b;
  InputFile(int b) : File(b * 2), b{b} {
    std::cout << "IFile ctor" << std::endl;
  }
};

struct OutputFile : virtual public File {
  int c;
  OutputFile(int c) : File(c * 3), c{c} {
    std::cout << "OFile ctor" << std::endl;
  }
};

struct IOFile : public InputFile, public OutputFile {
  int d;
  IOFile(int d) : File(d), InputFile(d * 5), OutputFile(d * 7) {
    std::cout << "IOFile ctor" << std::endl;
  }
};

int main() {
  OutputFile *pof = new OutputFile{13};
  InputFile *pif = dynamic_cast<InputFile *>(pof);
  assert(pif == nullptr);
  std::cout << std::hex << pif << std::dec << std::endl;

  try {
    OutputFile &rof = *pof;
    InputFile &rif = dynamic_cast<InputFile &>(rof);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  delete pof;
}
