#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::make_unique;
using std::unique_ptr;

template <typename T> struct MyClassDeleter {
  void operator()(T *t) { delete[] t; }
};

auto MyLambdaDeleter = [](auto *t) { delete[] t; };

template <typename T> void MyFuncDeleter(T *t) { delete[] t; }

int main() {
  int x = 1, y = 2;
  auto *ui0 = new int[1000];
  delete[] ui0;
  auto ui1 = make_unique<int[]>(1000);
  unique_ptr<int[]> ui2(new int[1000]());
  unique_ptr<int, MyClassDeleter<int>> ui3(new int[1000]());
  unique_ptr<int, decltype(MyLambdaDeleter)> ui4(new int[1000](),
                                                 MyLambdaDeleter);
  unique_ptr<int, decltype(&MyFuncDeleter<int>)> ui5(new int[1000](),
                                                     MyFuncDeleter<int>);

  auto MyLambdaDeleter2 = [x, y](auto *t) {
    cout << x << " " << y << endl;
    delete[] t;
  };
  unique_ptr<int, decltype(MyLambdaDeleter2)> ui6(new int[1000](),
                                                  MyLambdaDeleter2);

  cout << "\tui0:" << sizeof(ui0) << "\tui1:" << sizeof(ui1)
       << "\tui2:" << sizeof(ui2) << "\tui3:" << sizeof(ui3)
       << "\tui4:" << sizeof(ui4) << "\tui5:" << sizeof(ui5)
       << "\tui6:" << sizeof(ui6) << endl;
}
