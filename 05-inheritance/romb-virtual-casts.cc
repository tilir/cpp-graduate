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
  IOFile(int d) : File(d), InputFile(d * 5), OutputFile(d * 7) {
    std::cout << "IOFile ctor" << std::endl;
  }
};

int main() {
  IOFile *pe = new IOFile{11};
  std::cout << pe->a << std::endl;

  InputFile *pg = static_cast<InputFile *>(pe);
  std::cout << pg->a << std::endl;

  File *pf = static_cast<File *>(pg);
  std::cout << pf->a << std::endl;

  pe = static_cast<IOFile *>(pf);
  std::cout << pe->a << std::endl;
}
