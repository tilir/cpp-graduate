#define NIEB

// Nieblers ranges v3
#if defined(NIEB)
#include <range/v3/all.hpp>
// Future std
#elif defined(USESTD)
#include <ranges>
namespace ranges = std::ranges;
#endif