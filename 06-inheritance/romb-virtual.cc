//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Romb scheme -- virtual base classes
//
//----------------------------------------------------------------------------

#include <iostream>

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
  IOFile(int d) : File(d), InputFile(d * 5), OutputFile(d * 7), d{d} {
    std::cout << "IOFile ctor" << std::endl;
  }
};

#ifdef IO2
struct IOFile2 : public IOFile {
  int e;
  IOFile2(int e) : IOFile(e), e(e) {}
};
#endif

int main() {
  IOFile f(11);
  std::cout << f.InputFile::a << std::endl;
  std::cout << f.OutputFile::a << std::endl;
  std::cout << f.a << std::endl;

#ifdef IO1
  InputFile g(11);
  OutputFile h(11);
  std::cout << g.a << std::endl;
  std::cout << h.a << std::endl;
#endif

#ifdef IO2
  IOFile2 io2(11);
#endif
}
