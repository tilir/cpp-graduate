//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// unordered reserve example
//
//------------------------------------------------------------------------------

#include <iostream>
#include <unordered_map>

struct Foo {
  float x = 0.0f, y = 0.0f, z = 0.0f;
};

template <typename T> auto umap_size(const T &umap) {
  size_t count = 0;
  for (unsigned i = 0; i < umap.bucket_count(); ++i) {
    size_t bucket_size = umap.bucket_size(i);
    count += (bucket_size + 1);
  }
  return count;
}

template <typename T> void snapshot(const T &m1, const T &m2) {
  std::cout << "#1: size = " << umap_size(m1)
            << ", bcnt = " << m1.bucket_count() << std::endl;
  std::cout << "#2: size = " << umap_size(m2)
            << ", bcnt = " << m2.bucket_count() << std::endl;
}

int main() {
  std::unordered_map<int, Foo> mapNoReserve, mapReserve;
  snapshot(mapNoReserve, mapReserve);
  mapReserve.reserve(1000);
  std::cout << "After reserve:" << std::endl;
  snapshot(mapNoReserve, mapReserve);
  for (uint32_t i = 0; i < 1000; ++i) {
    mapNoReserve.emplace(i, Foo{});
    mapReserve.emplace(i, Foo{});
  }
  std::cout << "After fill in:" << std::endl;
  snapshot(mapNoReserve, mapReserve);
  mapNoReserve.rehash(1000);
  std::cout << "After rehash:" << std::endl;
  snapshot(mapNoReserve, mapReserve);
}
