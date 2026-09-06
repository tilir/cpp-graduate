//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example for LRU cache in C++
//
//----------------------------------------------------------------------------

#pragma once

#include <cstddef>
#include <list>
#include <unordered_map>
#include <utility>

namespace caches {

template <typename T, typename KeyT = int> struct cache_t {
  std::size_t sz_;
  // Each entry is {key, page}; most recently used entry is at the front.
  std::list<std::pair<KeyT, T>> cache_;

  using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
  std::unordered_map<KeyT, ListIt> hash_;

  explicit cache_t(std::size_t sz) : sz_(sz) {}

  std::size_t max_capacity() const { return sz_; }
  bool full() const { return (cache_.size() == sz_); }

  template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
    if (max_capacity() == 0)
      return false;

    auto hit = hash_.find(key);
    if (hit != hash_.end()) {
      auto eltit = hit->second;
      cache_.splice(cache_.begin(), cache_, eltit);
      return true;
    }

    // This key is absent, even if the cache contains other pages.
    T page = slow_get_page(key);

    if (full()) {
      hash_.erase(cache_.back().first);
      cache_.pop_back();
    }
    cache_.emplace_front(key, page);
    hash_.emplace(key, cache_.begin());
    return false;
  }
};

} // namespace caches
