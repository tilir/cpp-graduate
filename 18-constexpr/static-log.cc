//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Static logarithm in C++14 constexpr
//
//------------------------------------------------------------------------------

#include <iostream>

static inline constexpr size_t static_log(size_t N) {
  size_t pos = sizeof(size_t) * CHAR_BIT, mask = 0;

  if (N == 0)
    throw "N == 0 not supported";

  do {
    pos -= 1;
    mask = 1ull << pos;
  } while ((N & mask) != mask);

  // see if exact_log
  if (N != mask)
    pos += 1;

  return pos;
}

int main() {
  constexpr int log128 = static_log(128);
  std::cout << log128 << std::endl;

#ifdef ERR
  constexpr int log0 = static_log(0);
  std::cout << log0 << std::endl;
#endif

#ifdef RT
  for (size_t i = 1; i < 100; ++i)
    std::cout << i << ": " << static_log(i) << std::endl;
#endif
}
