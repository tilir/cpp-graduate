#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
  int arr[] = {2, 3, 5, 7, 11, 13, 17};
  std::ostream_iterator<int> os(std::cout, " ");
  std::vector<int> v(7);
  std::copy_n(arr, 7, v.begin());

  std::copy(v.begin(), v.end(), os);
  std::cout << std::endl;

  std::fill(v.begin(), v.end(), 0);
  std::copy_if(arr, arr + 7, v.begin(), [](int i) { return (i % 3) == 1; });

  std::copy(v.begin(), v.end(), os);
  std::cout << std::endl;
}