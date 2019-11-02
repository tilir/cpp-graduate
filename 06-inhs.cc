#include <iostream>

//------------------------------------------------------------------------------
//
//  Bases
//
//------------------------------------------------------------------------------

class PublicBase {
public:  
  int open = 0;
protected:
  int semiclosed = 1;
private:
  int closed = 2;
};

class ProtectedBase {
public:  
  int open = 3;
protected:
  int semiclosed = 4;
private:
  int closed = 5;
};

class PrivateBase {
public:  
  int open = 6;
protected:
  int semiclosed = 7;
private:
  int closed = 8;
};

//------------------------------------------------------------------------------
//
//  Derivation first level
//
//------------------------------------------------------------------------------

struct PublicDerive: public PublicBase {
  int get_open() const { return open; } 
  int get_semiclosed() const { return semiclosed; }
  // int get_closed() const { return closed; }
};

struct ProtectedDerive: protected ProtectedBase {
  int get_open() const { return open; } 
  int get_semiclosed() const { return semiclosed; }
  // int get_closed() const { return closed; }
};

struct PrivateDerive: private PrivateBase {
  int get_open() const { return open; } 
  int get_semiclosed() const { return semiclosed; }
  // int get_closed() const { return closed; }
};

//------------------------------------------------------------------------------
//
//  Derivation second level
//
//------------------------------------------------------------------------------

struct PublicDerive2: public PublicDerive {
  int get_open() const { return open; } 
  int get_semiclosed() const { return semiclosed; }
  // int get_closed() const { return closed; }
};

struct ProtectedDerive2: protected ProtectedDerive {
  int get_open() const { return open; } 
  int get_semiclosed() const { return semiclosed; }
  // int get_closed() const { return closed; }
};

struct PrivateDerive2: private PrivateDerive {
  // int get_open() const { return open; } 
  // int get_semiclosed() const { return semiclosed; }
  // int get_closed() const { return closed; }
};

//------------------------------------------------------------------------------
//
//  External access
//
//------------------------------------------------------------------------------

int main() {
  PublicDerive a;
  a.open;
  // a.semiclosed;
  // a.closed;
  
  ProtectedDerive b;
  // b.open;
  // b.semiclosed;
  // b.closed;

  PrivateDerive c;
  // c.open;
  // c.semiclosed;
  // c.closed;
}

