//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// self hash example
//
//------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <unordered_map>

struct S {
  std::string first_name, last_name;
  auto operator<=>(const S &) const = default;
};

// we have a right to add specialization to std
namespace std {
template <> struct hash<S> {
  std::size_t operator()(const S &K) const {
    std::hash<std::string> h;
    return h(K.first_name) + h(K.last_name); // ok????
  }
};
} // namespace std

int main() {
  std::unordered_map<S, std::string> Ump = {{{"Alice", "Chan"}, "Cat"},
                                            {{"Bob", "Dylan"}, "Dog"}};

  auto It = Ump.find(S{"Alice", "Chan"});
  std::cout << It->second << std::endl;
}