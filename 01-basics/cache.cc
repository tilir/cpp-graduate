//-----------------------------------------------------------------------------
//
// Source code for MIPT course on informatics//
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example for LRU cache in C++
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

template <typename T, typename KeyT = int> struct cache_t {
  size_t sz_;
  std::list<T> cache_;

  using ListIt = typename std::list<T>::iterator;
  std::unordered_map<KeyT, ListIt> hash_;

  cache_t(size_t sz) : sz_(sz) {}

  bool full() const { return (cache_.size() == sz_); }

  template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
    auto hit = hash_.find(key);
    if (hit == hash_.end()) {
      if (full()) {
        hash_.erase(cache_.back());
        cache_.pop_back();
      }
      cache_.push_front(slow_get_page(key));
      hash_[key] = cache_.begin();
      return false;
    }

    auto eltit = hit->second;
    if (eltit != cache_.begin())
      cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
    return true;
  }
};

// slow get page imitation
int slow_get_page_int(int key) { return key; }

int main() {
  int hits = 0;
  int n;
  size_t m;

  std::cin >> m >> n;
  assert(std::cin.good());
  cache_t<int> c{m};

  for (int i = 0; i < n; ++i) {
    int q;
    std::cin >> q;
    assert(std::cin.good());
    if (c.lookup_update(q, slow_get_page_int))
      hits += 1;
  }

  std::cout << hits << std::endl;
}
