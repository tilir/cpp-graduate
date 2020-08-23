#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

enum { NROWS = 10 };

int main() {
  std::ifstream f("poluslo.jpg", std::ifstream::binary);
  int nrows = 0;
  int printrow = 0;
  std::vector<unsigned char> buf;
  while (f) {
    unsigned char next;
    f >> std::noskipws >> next;
    buf.push_back(next);
    std::cout << std::hex << std::setfill('0') << std::setw(2) << +next;
    printrow += 1;
    if ((printrow % 16) == 0) {
      std::cout << std::endl;
      nrows += 1;
      if (nrows >= NROWS)
        break;
    } else if ((printrow % 2) == 0)
      std::cout << " ";
  }
}