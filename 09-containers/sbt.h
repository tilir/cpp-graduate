#ifndef SBT_GUARD__
#define SBT_GUARD__

#include <cassert>
#include <utility>
#include <vector>

using std::make_pair;
using std::pair;
using std::vector;

namespace SBT {

using rational_t = pair<int, int>;
using contfrac_t = vector<int>;

static inline rational_t top() { return make_pair(1, 1); }

pair<rational_t, rational_t> childs(rational_t);

template <typename T>
struct Rational_Greater {
  bool operator()(T x1, T x2) {
    double d1 = static_cast<double>(x1.first) / static_cast<double>(x1.second);
    double d2 = static_cast<double>(x2.first) / static_cast<double>(x2.second);
    return (d1 > d2);
  }
}; 

}

#endif