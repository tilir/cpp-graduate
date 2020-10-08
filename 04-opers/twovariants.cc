#include <iostream>

template <typename T> struct Quat {
  T x, y, z, w;

  void dump(std::ostream &os) const {
    os << x << " " << y << " " << z << " " << w;
  }

  Quat operator-() const { return Quat{x, -y, z, -w}; }
};

template <typename T> Quat<T> operator-(Quat<T> arg) {
  return Quat<T>{-arg.x, arg.y, -arg.z, arg.w};
}

int main() {
  Quat<float> q{1, 2, 3, 4};

  std::cout << "Before: " << std::endl;
  q.dump(std::cout);
  std::cout << std::endl;

  Quat<float> p = -q;

  std::cout << "After: " << std::endl;
  p.dump(std::cout);
  std::cout << std::endl;

  Quat<float> r = operator-(q);

  std::cout << "After explicit 1: " << std::endl;
  r.dump(std::cout);
  std::cout << std::endl;

  Quat<float> s = q.operator-();

  std::cout << "After explicit 2: " << std::endl;
  s.dump(std::cout);
  std::cout << std::endl;
}
