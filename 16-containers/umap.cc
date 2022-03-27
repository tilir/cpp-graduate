//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// basic unordered map
//
//------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <unordered_map>

int main() {
  std::unordered_map<std::string, std::string> Ump = {
      {"Alice", "Cat"}, {"Bob", "Dog"}, {"Camilla", "Dog"}, {"Alice", "Bird"}};

  auto It = Ump.find("Alice");
  std::cout << It->second << std::endl;

#if 0
  It->first = "Dave"; // Fail
#endif
  It->second = "Elephant"; // Ok

  It = Ump.find("Alice");
  std::cout << It->second << std::endl;
}