#include <tuple>

#include "sbt.h"

using std::swap;
using std::tie;

namespace SBT {

int gcd(int a, int b) {
  if (a < b)
    swap(a, b);
  while (b != 0) {
    int t = b;
    b = a % b;
    a = t;
  }
  return a;
}

auto to_contfrac(int x, int y) {
  assert (y != 0);
  vector<int> res;
  for (;;) {
    int i = x / y;
    res.push_back(i);
    int f = x % y;
    if (f == 0) break;
    x = y;
    y = f;
    auto cdiv = gcd(x, y);
    x /= cdiv;
    y /= cdiv;
  } 
  return res;
}

auto from_contfrac(vector<int> cf) {
  assert(!cf.empty());
  int x = cf.back();
  int y = 1;
  for (auto it = next(cf.rbegin()); it != cf.rend(); ++it) {
    int cur = *it;
    tie(x, y) = make_pair(cur*x + y, x);
    auto cdiv = gcd(x, y);
    x /= cdiv;
    y /= cdiv;
  }
  return make_pair(x, y);
}

template <typename T>
auto rational_min(T x1, T x2) {
  double d1 = static_cast<double>(x1.first) / static_cast<double>(x1.second);
  double d2 = static_cast<double>(x2.first) / static_cast<double>(x2.second);
  if (d1 > d2)
    return x2;
  return x1;
} 

template <typename T>
auto rational_max(T x1, T x2) {
  double d1 = static_cast<double>(x1.first) / static_cast<double>(x1.second);
  double d2 = static_cast<double>(x2.first) / static_cast<double>(x2.second);
  if (d1 <= d2)
    return x2;
  return x1;
} 

pair<rational_t, rational_t> 
childs(rational_t cur) {
  auto cf = to_contfrac(cur.first, cur.second);
  auto cf2 = cf;
  // one child is [a0, ..., ak + 1]
  // other is [a0, ..., ak - 1, 2]
  cf.back() += 1;
  cf2.back() -= 1;
  cf2.push_back(2);
  auto elt1 = from_contfrac(cf);
  auto elt2 = from_contfrac(cf2);
  return make_pair(rational_min(elt1, elt2), rational_max(elt1, elt2));
}


}