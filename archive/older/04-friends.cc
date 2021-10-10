class S {
  int x = 0;

public:
  int get_x() const { return x; }
  static int s_get_x(const S *s) { return s->x; }
  friend int f_get_x(const S *s);
};

int f_get_x(const S *s) { return s->x; }

int main() {
  S s1, s2;
  int x = 0;

  x += s1.get_x();
  x += s2.get_x();

  x += S::s_get_x(&s1);
  x += f_get_x(&s1);

  return x;
}