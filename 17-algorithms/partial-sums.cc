//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Partial sums vs inclusive scan example
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main() {
  std::ostream_iterator<int> os(std::cout, " ");
  std::vector<int> In = {8, 4, 2, 1};
  std::vector<int> Out;

  std::partial_sum(In.begin(), In.end(), std::back_inserter(Out));
  std::cout << "Partial sums: " << std::endl;
  std::copy(Out.begin(), Out.end(), os);
  std::cout << std::endl;

  Out.clear();
  std::inclusive_scan(In.begin(), In.end(), std::back_inserter(Out));
  std::cout << "Inclusive scan: " << std::endl;
  std::copy(Out.begin(), Out.end(), os);
  std::cout << std::endl;

  Out.clear();
  std::exclusive_scan(In.begin(), In.end(), std::back_inserter(Out), 0);
  std::cout << "Exclusive scan: " << std::endl;
  std::copy(Out.begin(), Out.end(), os);
  std::cout << std::endl;
}
