#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;

int main(void) {
  int n = 42;
  cout << n << endl;
  cout << std::hex << n << endl;
  cout << std::oct << n << endl;
  std::dec(cout);
  cout << std::setfill('.') << std::setw(10) << 42 << endl;
  return 0;
}
