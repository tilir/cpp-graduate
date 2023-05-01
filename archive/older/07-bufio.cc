#include <chrono>
#include <iostream>
#include <thread>

int main() {
#ifdef BUF
  setvbuf(stdout, NULL, _IOFBF, 1024);
#else
  setbuf(stdout, NULL);
#endif

  std::cout << "Hello, ";
  std::chrono::duration<int, std::milli> d(3000);
  std::this_thread::sleep_for(d);
  std::cout << "world" << std::endl;
}
