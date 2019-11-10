#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::getline;

int main() {
  std::string mystr;
  int num;
  cout << "Please enter a number:\n";
  cin >> num;
  cout << "Your number is: " << num << "\n";
  cout << "Please enter your name: \n";
  cin.ignore();
  getline(cin, mystr);
  cout << "Your name is: " << mystr << "\n";
}
