#include <iostream>

int main() {
  int n, cmax = 0, xmax = 0;
  std::cin >> n;
  for (int x = 2; x < n; ++x) {
    int y = x;
    int c = 0;
    while (y > 1) {
      c += 1;
      if ((y % 2) == 0)
        y = y / 2;
      else
        y = 3 * y + 1;
    }
    if (c > cmax) {
      cmax = c;
      xmax = x;
    }
  }
  std::cout << xmax << std::endl << cmax << std::endl;
}
