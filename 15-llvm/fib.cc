int fib(int n) {
  int a = 1, b = 1, i;
  for (i = 0; i < n; ++i) {
    int tmp = a;
    a = a + b;
    b = tmp;
  }
  return b;
}
