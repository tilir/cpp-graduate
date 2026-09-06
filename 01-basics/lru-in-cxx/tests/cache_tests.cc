#include "cache.hpp"

#include <iostream>
#include <cstdlib>
#include <string>

void check(bool condition) {
  if (!condition) {
    std::cerr << "Cache check failed\n";
    std::exit(1);
  }
}

int main() {
  caches::cache_t<std::string> cache{2};
  int calls = 0;
  auto load = [&](int key) {
    ++calls;
    return "page " + std::to_string(key);
  };
  check(!cache.lookup_update(1, load));
  check(calls == 1);
  // A missing key must load a page even when the cache is nonempty.
  check(!cache.lookup_update(2, load));
  check(calls == 2);
  check(cache.hash_.at(2)->second == "page 2");
  check(cache.full());
  check(cache.lookup_update(1, load));
  check(calls == 2);
  check(!cache.lookup_update(3, load));
  check(calls == 3);
  check(cache.hash_.count(2) == 0);
  check(cache.cache_.back().first == 1);
  check(cache.hash_.at(1)->second == "page 1");

  auto replacement = [](int) { return std::string("replacement"); };
  check(cache.lookup_update(1, replacement));
  check(cache.cache_.front().second == "page 1");
  check(!cache.lookup_update(2, replacement));
  check(cache.cache_.front().second == "replacement");

  caches::cache_t<std::string> empty{0};
  const int previous_calls = calls;
  check(!empty.lookup_update(1, load));
  check(!empty.lookup_update(1, load));
  check(calls == previous_calls);
  check(empty.cache_.empty() && empty.hash_.empty());

  caches::cache_t<int, std::string> generic{1};
  auto string_loader = [](std::string key) { return static_cast<int>(key.size()); };
  check(!generic.lookup_update("answer", string_loader));
  check(generic.lookup_update("answer", string_loader));
  check(generic.cache_.front().second == 6);
  return 0;
}
