#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>

using std::cout;
using std::endl;
using std::runtime_error;

template <typename T> void
construct (T *p, const T& rhs) 
{
  new (p) T (rhs);
}

template <class T> void
destroy(T* p)
{
  p->~T();
}

template <typename FwdIter> 
void destroy (FwdIter first, FwdIter last) 
{
   while (first++ != last)
     destroy (&*first);
}

template <typename T>
struct MyVectorBuf 
{
  MyVectorBuf(const MyVectorBuf&); // = delete;
  MyVectorBuf& operator= (const MyVectorBuf&); // = delete;
protected:
  T *arr_;
  size_t size_, used_;

  MyVectorBuf(size_t sz = 0) : 
    arr_((sz == 0) ? NULL : 
      static_cast<T*>(::operator new(sizeof(T) * sz))), 
    size_(sz), used_(0) {}

  ~MyVectorBuf() {
    destroy(arr_, arr_ + used_);
    ::operator delete(arr_);
  }

  void swap(MyVectorBuf &rhs) /* noexcept */ {
    std::swap (arr_, rhs.arr_); 
    std::swap (size_, rhs.size_); 
    std::swap (used_, rhs.used_);
  }
};

template <typename T> struct MyVector : 
                       private MyVectorBuf<T> 
{
  using MyVectorBuf<T>::used_;
  using MyVectorBuf<T>::size_;
  using MyVectorBuf<T>::arr_;

  MyVector (size_t sz): MyVectorBuf<T>(sz) {}

  MyVector (const MyVector &rhs) : MyVectorBuf<T>(rhs.used_) 
  {
    while (used_ < rhs.used_) {
      construct (arr_ + used_, rhs.arr_[used_]);
      used_ += 1;
    }
  }

  MyVector& operator= (const MyVector &rhs) 
  {
    MyVector tmp (rhs); 
    this->swap(tmp);
    return *this;
  }

  T top () const 
  { 
    if (used_ < 1) throw runtime_error("Vector is empty"); 
    return arr_[used_ - 1];
  }

  void pop () 
  {
    if (used_ < 1) throw runtime_error("Vector is empty");
    used_ -= 1;
    destroy(arr_ + used_);
  }

  void push(const T& t) 
  {
    assert (used_ <= size_);
    if (used_ == size_) {
      MyVector tmp (size_*2 + 1);
      while (tmp.size() < used_)
        tmp.push(arr_[tmp.size()]);
      tmp.push(t);
      this->swap(tmp);
    }
    else {
      construct(arr_ + used_, t);
      used_ += 1;
    }
  }

  size_t size() const { return used_; }
  size_t capacity() const { return size_; }
};

class RefBind {
  static int g;
  int& ref;
public:
  RefBind() : ref(g) {}
  RefBind(int x): ref(x) {}
  int get() const { return ref; }
};

int RefBind::g = 0;

int
main() {
  MyVector<int> v(10);
  for (int i = 0; i < 30; ++i)
    v.push(i);
  MyVector<int> v2(v);
  MyVector<int> v3(10);
  for (int i = 0; i < 5; ++i)
    v.pop();
  v3 = v;
  for (int i = 0; i < 5; ++i)
    v.pop();

  cout << v.size() << endl;
  cout << v2.size() << endl;
  cout << v3.size() << endl;

#if VECVEC
  MyVector<MyVector<int>> vv(1);
  vv.push(v);
  vv.push(v2);
  vv.push(v3);
  cout << vv.size() << endl;
#endif

#if VECREF
  MyVector<RefBind> vr(10);
  vr.push(10);
  MyVector<RefBind> vr2(vr);
  cout << vr2.size() << endl;
#endif
}
