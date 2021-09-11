//-----------------------------------------------------------------------------
//
// Source code for MIPT course on informatics//
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Triangles demo: grouping together data and processing
//
//-----------------------------------------------------------------------------

#include <cmath>
#include <iostream>

struct Point {
  double x, y;
};

struct Triangle {
  Point pts[3];
  double square() {
    double sq = pts[0].x * (pts[1].y - pts[2].y) +
                pts[1].x * (pts[2].y - pts[0].y) +
                pts[2].x * (pts[0].y - pts[1].y);
    return std::abs(sq) / 2.0;
  }
};

int main() {
  struct Triangle t;
  t.pts[0] = Point{1.0, 1.0};
  t.pts[1] = Point{3.0, 3.0};
  t.pts[2] = Point{1.0, 2.0};
  double square = t.square();
  std::cout << square << std::endl;
}
