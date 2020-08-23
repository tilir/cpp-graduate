#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main() {
#ifdef BUF
  setvbuf(stdout, NULL, _IOFBF, 1024);
#else
  setbuf (stdout, NULL);
#endif

  printf("%s, ", "Hello");
  sleep(5);
  printf("%s!\n", "world");
}
