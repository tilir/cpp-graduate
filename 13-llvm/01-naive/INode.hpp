//------------------------------------------------------------------------------
//
// INode -- main node interfaces, operations enum and ctor functions for nodes
//
//------------------------------------------------------------------------------

#pragma once

#include "llvm/IR/Value.h"
#include <string>

// node interface
struct INode {
  virtual int calc() = 0;
  virtual llvm::Value *codegen() = 0;
  virtual void dump() const = 0;
  virtual ~INode() {}
};

// scope interface
struct IScope : public INode {
  virtual IScope *push() = 0;
  virtual IScope *resetScope() const = 0;
  virtual void addBranch(INode *branch) = 0;
  virtual INode *access(std::string const &var_name) = 0;
  virtual INode *visible(std::string const &var_name) = 0;
};

// operations
enum class Ops {
  Plus,
  Minus,
  Assign,
  Greater,
  Less,
  GreaterEq,
  LessEq,
  StdOut,
  StdIn,
  Equal,
  NotEqual,
  Div,
  Mul,
  Mod,
  Not,
  And,
  Or
};

// ctor functions
INode *make_value(int);
INode *make_op(INode *l, Ops o, INode *r);
INode *make_while(INode *o, INode *s);
INode *make_if(INode *o, INode *s);
IScope *create_scope();
