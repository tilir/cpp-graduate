// a.exe > out.txt 2> err.txt

#include <stdio.h>

int main() {
  printf("%s\n", "Hello, out!");
  fprintf(stderr, "%s\n", "Hello, err!");
}
