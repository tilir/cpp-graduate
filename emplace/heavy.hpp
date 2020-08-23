#ifndef HEAVY_GUARD__
#define HEAVY_GUARD__

#include <iostream>

using std::cout;
using std::endl;

class Heavy
{
  int n;
  int *t;
public:
  Heavy (int sz) : n(sz), t(new int[n]) {
    cout << "Heavy created" << endl;
  }

  Heavy(const Heavy &rhs) : n(rhs.n), t(new int[n]) {
    cout << "Heavy copied" << endl;
    memcpy (t, rhs.t, n*sizeof(int));
  }

  ~Heavy () {
    cout << "Heavy destroyed" << endl;
    delete [] t;
  }
  
  void print(std::ostream &strm) const {
    strm << "Heavy: " << n;
  }
};

std::ostream& operator << (std::ostream& strm, const Heavy& h) {
  h.print(strm);
  return strm;
}

#endif
