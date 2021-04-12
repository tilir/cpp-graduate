#ifndef MYRES_GUARD_
#define MYRES_GUARD_

#include <cassert>
#include <iostream>

using std::cout;
using std::endl;

class MyRes {
  int content_;
  int valid_;

public:
  MyRes(int c) : content_(c), valid_(1) { 
    cout << "MyRes ctor(int): " << c << endl; 
  }

  MyRes(int c, double) : content_(c), valid_(1) { 
    cout << "MyRes ctor (int, double)" << endl; 
  }

  MyRes(const MyRes& rhs) : 
    content_(rhs.content_), valid_(rhs.valid_) 
  { 
    cout << "MyRes copy ctor" << endl; 
    if (valid_ == 0)
      cout << "Copying moved-from object" << endl; 
  }

  MyRes(MyRes&& rhs) noexcept :
    content_(rhs.content_), valid_(rhs.valid_) 
  {    
    cout << "MyRes move ctor" << endl; 
    if (valid_ == 0)
      cout << "Moving from moved-from object!" << endl; 
    rhs.content_ = 0;
    rhs.valid_ = 0;
  }

  MyRes& operator=(const MyRes& rhs) { 
    cout << "MyRes copy assign" << endl;
    content_ = rhs.content_;    
    valid_ = rhs.valid_;
    return *this; 
  }

  MyRes& operator=(MyRes&& rhs) noexcept { 
    cout << "MyRes move assign" << endl;
    if (&rhs != this) {
      valid_ = rhs.valid_;
      content_ = rhs.content_;
      rhs.valid_ = 0;
      rhs.content_ = 0;
    }
    if (valid_ == 0)
      cout << "Moving from moved-from object!" << endl; 
    return *this; 
  }

  ~MyRes() { 
    cout << "MyRes dtor: " << content_ << endl;
  }

  void use() /* intentionally non-const */ { 
    cout << "MyRes used: " << content_ << endl; 
    if (valid_ == 0)
      cout << "Using moved-from object" << endl;
  }
  
  int content() const { return content_; }
};

#endif