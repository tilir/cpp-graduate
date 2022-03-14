n = ?;
x = 2;
cmax = 0;
xmax = 0;

while (x < n) {
  c = 0;
  y = x;
  while (y > 1) {
    c = c + 1;
    t = 0;
    if ((y % 2) == 0) {
      t = 1;
      y = y / 2;
    } 
    if ((t == 0) && ((y % 2) != 0)) {
      y = 3 * y + 1;
    }
  }

  if (c > cmax) {
    xmax = x;
    cmax = c; 
  }

  x = x + 1;
}

print xmax;
print cmax;

