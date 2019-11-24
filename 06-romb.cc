#include <iostream>

struct File {
  int a;
  File(int a) : a{a} {}
  virtual ~File() {}
};

struct InputFile : public File {
  int b;
  InputFile(int b) : File(b * 2), b{b} {}
};

struct OutputFile : public File {
  int c;
  OutputFile(int c) : File(c * 3), c{c} {}
};

struct IOFile : public InputFile, public OutputFile {
  int d;
  IOFile(int d) : InputFile(d * 5), OutputFile(d * 7) {}
};

int main() {
  IOFile f(11);
  std::cout << f.a << std::endl;
  std::cout << f.InputFile::a << std::endl;
  std::cout << f.OutputFile::a << std::endl;
}
