#include <iostream>
#include <string>

int main() {
  volatile std::string s{"Hello"};

  char c = s[2];
  s += "world";
  std::cout << s << std::endl;
  std::cout << s.c_str() << std::endl;
}