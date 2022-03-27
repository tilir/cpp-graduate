//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// string view
//
//------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <string_view>

std::string_view trim(std::string_view sv) {
  auto trimfst = sv.find_first_not_of(" ");
  auto minsz = std::min(trimfst, sv.size());
  sv.remove_prefix(minsz);

  auto trimlst = sv.find_last_not_of(" ");
  auto sz = sv.size() - 1;
  minsz = std::min(trimlst, sz);
  sv.remove_suffix(sz - minsz);
  return sv;
}

int main() {
  std::string str = "   trim me  ";
  std::string_view sv = trim(str); // OK?
  std::cout << str << std::endl;
  std::cout << sv << std::endl;

  sv = trim("trim me"); // OK?
  std::cout << sv << std::endl;

  sv = trim(std::string("trim me")); // OK?
  std::cout << sv << std::endl;
}