//------------------------------------------------------------------------------
//
// pcllib.cc -- ParaCL driver
//
// main calls __pcl_start
// defines __pcl_print and __pcl_scan
//
//------------------------------------------------------------------------------

#include <iostream>

extern "C" void __pcl_start();

extern "C" void __pcl_print(int n) { std::cout << n << std::endl; }

extern "C" int __pcl_scan() {
  int n;
  std::cin >> n;
  if (!std::cin) {
    std::cerr << "Problem reading stdin\n";
    exit(1);
  }
  return n;
}

int main() { __pcl_start(); }
