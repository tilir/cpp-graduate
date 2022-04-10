//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// toupper and its fixes
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

int main() {
  std::string s = "hello";
#if 0
  std::transform(s.begin(), s.end(), s.begin(), std::toupper); // FAIL
#endif
  std::transform(s.begin(), s.end(), s.begin(),
                 ::toupper); // FAIL-ISH style: C function used
  std::cout << s << std::endl;
  std::transform(s.begin(), s.end(), s.begin(),
                 [](auto &&c) { return std::tolower(c); }); // OK
  std::cout << s << std::endl;
}
