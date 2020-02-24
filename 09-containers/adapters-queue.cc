#include <iostream>
#include <queue>
#include <stack>

#include "sbt.h"

using std::cout;
using std::deque;
using std::endl;
using std::priority_queue;
using std::stack;
using std::queue;

void test_stack() {
  stack<SBT::rational_t> s;
  s.push(SBT::top());
  while(!s.empty()) {
    auto cur = s.top();
    s.pop();
    if ((cur.first < 4) && (cur.second < 4)) {
      auto childs = SBT::childs(cur);
      s.push(childs.second);
      s.push(childs.first);
    }
    cout << cur.first << "/" << cur.second << endl;
  }
}

void test_queue() {
  queue<SBT::rational_t> q;
  q.push(SBT::top());
  while(!q.empty()) {
    auto cur = q.front();
    q.pop();
    if ((cur.first < 4) && (cur.second < 4)) {
      auto childs = SBT::childs(cur);
      q.push(childs.first);
      q.push(childs.second);
    }
    cout << cur.first << "/" << cur.second << endl;
  }
}

void test_priqueue() {
  priority_queue<SBT::rational_t, deque<SBT::rational_t>, 
                 SBT::Rational_Greater<SBT::rational_t>> pq;

  queue<SBT::rational_t> q;
  q.push(SBT::top());
  while(!q.empty()) {
    auto cur = q.front();
    q.pop();
    if ((cur.first < 4) && (cur.second < 4)) {
      auto childs = SBT::childs(cur);
      q.push(childs.first);
      q.push(childs.second);
    }
    pq.push(cur);
  }

  while (!pq.empty()) {
    auto cur = pq.top();
    pq.pop();
    cout << cur.first << "/" << cur.second << endl;
  }
}

int
main () {
  cout << "Stern -- Brocot tree using stack:" << endl;
  test_stack();
  cout << "Stern -- Brocot tree using queue:" << endl;
  test_queue();
  cout << "Stern -- Brocot tree using priority queue:" << endl;
  test_priqueue();
}
