#include <iostream>

struct File {
  int a;
  File(int a) : a{a} { std::cout << "File ctor" << std::endl; }
  virtual ~File() {}
};

struct DFile : public File {
  DFile() : File(5) {}
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

  IOFile *piof = new IOFile{11};
  std::cout << std::hex << "piof = " << piof << ": " << std::dec;
  std::cout << piof->d << std::endl;

  File *pf = static_cast<File *>(piof);
  std::cout << std::hex << "pf = " << pf << ": " << std::dec;
  std::cout << pf->a << std::endl;

  InputFile *pif = dynamic_cast<InputFile *>(pf);
  std::cout << std::hex << "pif = " << pif << ": " << std::dec;
  std::cout << pif->b << std::endl;

  OutputFile *pof = dynamic_cast<OutputFile *>(pf);
  std::cout << std::hex << "pof = " << pof << ": " << std::dec;
  std::cout << pof->c << std::endl;

  pif = dynamic_cast<InputFile *>(pof);
  std::cout << std::hex << "pif = " << pif << ": " << std::dec;
  std::cout << pif->b << std::endl;

  piof = dynamic_cast<IOFile *>(pf);
  std::cout << std::hex << "piof = " << piof << ": " << std::dec;
  std::cout << piof->d << std::endl;

  delete piof;

  pof = new OutputFile{13};
  InputFile &rif = dynamic_cast<InputFile &>(*pof); // ?
}
