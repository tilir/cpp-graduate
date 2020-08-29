//----------------------------------------------------------------------------
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

struct cache_t {
  size_t sz_;
  std::list<int> cache_;

  using ListIt = std::list<int>::iterator;
  std::unordered_map<int, ListIt> hash_;

  cache_t(size_t sz) : sz_(sz) {}

  bool full() const {
    return (cache_.size() == sz_);
  }

  bool lookup(int elem) {
    auto hit = hash_.find(elem);
    if (hit == hash_.end()) {
      if (full()) {
        hash_.erase(cache_.back());
        cache_.pop_back();
      }
      cache_.push_front(elem);
      hash_[elem] = cache_.begin();
      return false;
    }

    auto eltit = hit->second;
    if (eltit != cache_.begin())
      cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
    return true;
  }
};

int main() {
  int hits = 0;
  int n, m;

  std::cin >> m >> n;
  assert(std::cin.good());
  cache_t c{m};

  for (int i = 0; i < n; ++i) {
    int q;    
    std::cin >> q;
    assert(std::cin.good());
    if (c.lookup(q))
      hits += 1;
  }

  std::cout << hits << std::endl;
}
