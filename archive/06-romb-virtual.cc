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
  IOFile f(11);
  std::cout << f.InputFile::a << std::endl;
  std::cout << f.OutputFile::a << std::endl;
  std::cout << f.a << std::endl;

  InputFile g(13);
  OutputFile h(17);
  std::cout << g.a << std::endl;
  std::cout << h.a << std::endl;
}
