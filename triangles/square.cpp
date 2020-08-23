#include "lingeo.hpp"
#include <iostream>
#include <vector>

polygon_t input_triangle() {
  std::vector<point_t> verts(3);
  point_t temp;
  for (int i = 0; i < 3; i++) {
    std::cin >> temp.x;
    std::cin >> temp.y;
    verts[i] = temp;
  }
  polygon_t ret{verts};
  return ret;
}

int main() {
  polygon_t triangle1 = input_triangle();
  polygon_t triangle2 = input_triangle();
  polygon_t inter = triangle1.get_poly_intersection(triangle2);
  std::cout << inter.square() << std::endl;
}
