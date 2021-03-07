#include <deque>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

using std::cin;
using std::cout;
using std::deque;
using std::endl;
using std::forward_list;
using std::istream_iterator;
using std::iterator_traits;
using std::list;
using std::ostream_iterator;
using std::vector;

std::ostream &operator<<(std::ostream &out, std::random_access_iterator_tag) {
  out << "random access";
  return out;
}

std::ostream &operator<<(std::ostream &out, std::bidirectional_iterator_tag) {
  out << "bidirectional";
  return out;
}

std::ostream &operator<<(std::ostream &out, std::forward_iterator_tag) {
  out << "forward";
  return out;
}

std::ostream &operator<<(std::ostream &out, std::input_iterator_tag) {
  out << "input";
  return out;
}

std::ostream &operator<<(std::ostream &out, std::output_iterator_tag) {
  out << "output";
  return out;
}

template <typename Iter> void print_iterator_type() {
  cout << typename iterator_traits<Iter>::iterator_category{} << endl;
}

int main() {
  print_iterator_type<typename std::deque<int>::iterator>();
  print_iterator_type<typename std::forward_list<int>::iterator>();
  print_iterator_type<typename std::list<int>::iterator>();
  print_iterator_type<typename std::vector<int>::iterator>();
  print_iterator_type<std::istream_iterator<int>>();
  print_iterator_type<std::ostream_iterator<int>>();
}
