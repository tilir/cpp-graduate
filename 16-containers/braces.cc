//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// auto braces
//
//------------------------------------------------------------------------------

#include <iostream>
#include <unordered_map>

int main() {
  std::unordered_map<int, int> m = {{1, 20}, {100, 30}};
  auto x = m[100];
  auto [it, b] = m.emplace(100, int{});
  if (!b)
    it = m.find(100);
  std::cout << x << std::endl;
  std::cout << it->second << std::endl;
}
