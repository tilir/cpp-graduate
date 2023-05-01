#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::forward_as_tuple;
using std::make_tuple;
using std::pair;
using std::vector;

int main() {
  pair<vector<int>, vector<int>> p(std::piecewise_construct, forward_as_tuple(),
                                   forward_as_tuple(2, 3));

  cout << "(" << p.first.size() << ", " << p.second.size() << ")" << endl;

  pair<vector<int>, vector<int>> p2(std::piecewise_construct, make_tuple(),
                                    make_tuple(2, 3));

  cout << "(" << p2.first.size() << ", " << p2.second.size() << ")" << endl;
}
