//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Group theoretical algorithms. Currently only orbit.
//
//------------------------------------------------------------------------------

#pragma once

#include <unordered_map>
#include <unordered_set>

#include "perms.hpp"

// simple orbit, using unordered sets
template <typename T, typename RandIt>
auto orbit(T num, RandIt gensbeg, RandIt gensend) {
  std::unordered_set<T> orbit, next = {num};
  while (!next.empty()) {
    std::unordered_set<T> tmp{};
    orbit.insert(next.begin(), next.end());
    for (const auto &elem : next)
      for (auto igen = gensbeg; igen != gensend; ++igen)
        if (auto newelem = igen->apply(elem); orbit.count(newelem) == 0)
          tmp.insert(newelem);
    next.swap(tmp);
  }
  return orbit;
}

// orbit with reps, using unordered maps
template <typename T, typename RandIt>
auto orbit_reps(T num, RandIt gensbeg, RandIt gensend, size_t to_reserve = 0) {
  std::unordered_map<T, Permutation<T>> orbit, next{{num, {}}};
  if (to_reserve)
    orbit.reserve(to_reserve);
  while (!next.empty()) {
    std::unordered_map<T, Permutation<T>> tmp{};
    orbit.insert(next.begin(), next.end());
    for (auto &&[elem, curgen] : next)
      for (auto igen = gensbeg; igen != gensend; ++igen)
        if (auto newelem = igen->apply(elem);
            orbit.find(newelem) == orbit.end())
          tmp.try_emplace(newelem, product(curgen, *igen));
    next.swap(tmp);
  }

  return orbit;
}
