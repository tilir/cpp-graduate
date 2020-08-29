//----------------------------------------------------------------------------
//
//  Example for triangle intersection
//  Originated from homework of D. Bushev
//
//----------------------------------------------------------------------------

#pragma once
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#define flt_tolerance 0.00001
#define inter_area_width 100.0

enum area_t { LEFT_SIDE, INTER_SIDE, RIGHT_SIDE };

//----------------------------------------------------------------------------
//
// point_t -- basic (x, y) point
//
//----------------------------------------------------------------------------

struct point_t {
  float x = NAN, y = NAN;

  void print() const { std::cout << "(" << x << " ; " << y << ")"; }

  bool valid() const { return !(x != x || y != y); }

  bool equal(const point_t &rhs) const {
    assert(valid() && rhs.valid());
    return (std::abs(x - rhs.x) < flt_tolerance) &&
           (std::abs(y - rhs.y) < flt_tolerance);
  }
};

//----------------------------------------------------------------------------
//
// line_t -- line in form of ax + by + c = 0
//
//----------------------------------------------------------------------------

struct line_t {
  float a = -1.0f, b = 1.0f, c = 0.0f;

  line_t(const point_t &p1, const point_t &p2) {
    float angle = std::atan((p2.y - p1.y) / (p2.x - p1.x));
    float sin_angle = std::sin(angle);
    float cos_angle = std::sqrt(1.0 - sin_angle * sin_angle);
    point_t normal_vect{-sin_angle, cos_angle};
    a = normal_vect.x;
    b = normal_vect.y;
    c = -(p1.x * normal_vect.x + p1.y * normal_vect.y);
  }

  enum area_t get_side_area(const point_t &point) const {
    float side_offset = a * point.x + b * point.y + c;
    if (side_offset > 0.0 + flt_tolerance * inter_area_width)
      return LEFT_SIDE;

    if (side_offset > 0.0 - flt_tolerance * inter_area_width)
      return INTER_SIDE;

    return RIGHT_SIDE;
  }

  bool separates(const point_t &pnt1, const point_t &pnt2) const {
    enum area_t side1 = get_side_area(pnt1);
    enum area_t side2 = get_side_area(pnt2);
    if (side1 == INTER_SIDE || side2 == INTER_SIDE)
      return false;
    return !(side1 == side2);
  }

  bool valid() const { return !(a != a || b != b || c != c); }

  bool intersect(const line_t &another) const {
    return (std::abs(a * another.b - another.a * b) >= flt_tolerance);
  }

  point_t point_of_intersect(const line_t &another) const {
    if (!intersect(another))
      return point_t{};
    float det = (a * another.b - another.a * b);
    float det1 = ((-c) * another.b - (-another.c) * b);
    float det2 = (a * (-another.c) - another.a * (-c));
    return {det1 / det, det2 / det};
  }

  void print() const {
    const char *signb = (b > 0.0) ? "+" : "";
    const char *signc = (c > 0.0) ? "+" : "";
    std::cout << a << "x " << signb << b << "y " << signc << c << " = 0";
  }
};

//----------------------------------------------------------------------------
//
// polygon_t -- polygon as point array
//
//----------------------------------------------------------------------------

struct polygon_t {
  std::vector<point_t> vertices;

  float square() const {
    if (vertices.size() < 3)
      return 0;

    float sum1 = 0, sum2 = 0;
    for (size_t i = 0; i < vertices.size(); i++) {
      sum1 += vertices[i].x * vertices[(i + 1) % vertices.size()].y;
      sum2 -= vertices[(i + 1) % vertices.size()].x * vertices[i].y;
    }

    float result = (sum1 + sum2) / 2.0;
    if (result < 0.0)
      result = -result;

    return result;
  }

  void print() const {
    std::cout << "Polygon has " << vertices.size() << " vertices: ";
    for (size_t i = 0; i < vertices.size(); i++) {
      if (i != 0)
        std::cout << ", ";
      vertices[i].print();
    }
    std::cout << std::endl;
  }

  // nth side constructed by n'th and (n + 1)'th vertices
  line_t get_side(int index) const {
    assert(!vertices.empty());
    auto vsz = vertices.size();
    if (index < 0)
      index += (1 + (index / vsz)) * vsz;
    line_t ret(vertices[index % vsz], vertices[(index + 1) % vsz]);
    return ret;
  }

  bool valid() const {
    if (vertices.size() == 0)
      return false;

    for (size_t i = 0; i < vertices.size(); i++)
      if (!vertices[i].valid())
        return false;

    return true;
  }

  // tells if this polygon can be separate from another by any line holding a
  // side of this polygon
  bool separable_from(const polygon_t &another) const {
    for (size_t i = 0; i < vertices.size(); i++) {
      line_t div_line = get_side(i);

      enum area_t half_space =
          div_line.get_side_area(vertices[(i + 2) % vertices.size()]);

      if (half_space == INTER_SIDE)
        continue;

      bool flag = true;
      for (size_t j = 0; j < another.vertices.size(); j++) {
        point_t vertex = another.vertices[j];
        if (half_space == div_line.get_side_area(vertex)) {
          flag = false;
          break;
        }
      }
      if (flag)
        return true;
    }
    return false;
  }

  // return the point of polygon side intersection with line if it exists,
  // returns non-valid point otherwise
  point_t side_line_intersect(int index, const line_t &line) const {
    point_t ret;
    size_t vsz = vertices.size();
    if (index < 0)
      index += (1 + (index / vsz)) * vsz;

    if (!line.separates(vertices[index % vsz], vertices[(index + 1) % vsz]))
      return ret;

    line_t poly_side = get_side(index);

    return line.point_of_intersect(poly_side);
  }

  // returns the cutted polygon (which half of polygon will be cutted defines by
  // half_space_pt)
  polygon_t cut_poly_by_line(const line_t &line,
                             const point_t &half_space_pt) const {
    enum area_t side = line.get_side_area(half_space_pt);
    assert(side != INTER_SIDE);
    polygon_t ret;

    for (size_t i = 0; i < vertices.size(); i++) {
      enum area_t vert_side = line.get_side_area(vertices[i]);

      // half_space_pt side points and inter side points are both acceptable
      if ((side == vert_side || vert_side == INTER_SIDE) &&
          !ret.holding(vertices[i]))
        ret.add(vertices[i]);

      point_t pintersect = side_line_intersect(i, line);
      if (pintersect.valid())
        ret.add(pintersect);
    }
    return ret;
  }

  polygon_t get_poly_intersection(const polygon_t &rhs) const {
    if (!intersect(rhs))
      return polygon_t{};

    polygon_t another = rhs;
    for (size_t i = 0; i < vertices.size(); i++) {
      line_t side = get_side(i);
      another =
          another.cut_poly_by_line(side, vertices[(i + 2) % vertices.size()]);
    }
    return another;
  }

  bool holding(const point_t &vert) const {
    for (size_t i = 0; i < vertices.size(); i++)
      if (vert.equal(vertices[i]))
        return true;
    return false;
  }

  bool intersect(const polygon_t &another) const {
    return !(separable_from(another) || another.separable_from(*this));
  }

  void add(const point_t &vert) {
    if (!holding(vert))
      vertices.push_back(vert);
  }
};
