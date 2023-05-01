// compile with: g++ --std=c++17 07-poluslo-fs.cc
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

const size_t NROWS = 10;
const size_t ROWSIZE = 16;
const char *FNAME = "poluslo.jpg";

int main() {
  fs::path p = fs::current_path() / FNAME;
  size_t sz = fs::file_size(p);
  std::cout << "File size = " << sz << std::endl;

  sz = std::min(sz, NROWS * ROWSIZE);
  std::vector<unsigned char> buf;
  buf.reserve(sz);

  std::ifstream f(FNAME, std::ifstream::binary);
  size_t nrows = 0;
  size_t printrow = 0;
  for (;;) {
    unsigned char next;
    f >> std::noskipws >> next;
    if (!f.good())
      break;
    buf.push_back(next);
    std::cout << std::hex << std::setfill('0') << std::setw(2) << +next;
    printrow += 1;
    if ((printrow % ROWSIZE) == 0) {
      std::cout << std::endl;
      nrows += 1;
      if (nrows >= NROWS)
        break;
    } else if ((printrow % 2) == 0)
      std::cout << " ";
  }
}
