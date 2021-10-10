#include <cassert>
#include <iostream>
#include <typeinfo>

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
  std::cout << typeid(IOFile).name() << std::endl;
  std::cout << typeid(OutputFile).name() << std::endl;
  std::cout << typeid(InputFile).name() << std::endl;
  std::cout << std::endl;

  IOFile *piof = new IOFile{5};
  OutputFile *pof = new OutputFile{5};
  InputFile *pif = new InputFile{5};

  std::cout << typeid(*piof).name() << std::endl;
  std::cout << typeid(*pof).name() << std::endl;
  std::cout << typeid(*pif).name() << std::endl;
  std::cout << std::endl;

  delete pif;
  delete pof;

  pof = static_cast<OutputFile *>(piof);
  assert(typeid(*pof) == typeid(IOFile));
  std::cout << typeid(*pof).name() << std::endl;
  std::cout << typeid(pof).name() << std::endl;

  pif = static_cast<InputFile *>(piof);
  std::cout << typeid(*pif).name() << std::endl;
  std::cout << typeid(pif).name() << std::endl;

  delete piof;
}
