#include <array>
#include <iostream>

// primary template
template <int Sum, char... Chars> struct binparser;

// account for '0'
template <int Sum, char... Rest> struct binparser<Sum, '0', Rest...> {
  static constexpr int value = binparser<Sum * 2, Rest...>::value;
};

// account for '1'
template <int Sum, char... Rest> struct binparser<Sum, '1', Rest...> {
  static constexpr int value = binparser<Sum * 2 + 1, Rest...>::value;
};

// end recursion
template <int Sum> struct binparser<Sum> { static constexpr int value = Sum; };

// binary suffix
template <char... Chars> constexpr int operator"" _binary() {
  return binparser<0, Chars...>::value;
}

int main() { std::cout << 11001010_binary << std::endl; }
