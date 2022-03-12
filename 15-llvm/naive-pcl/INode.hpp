//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// INode -- main node interfaces, operations enum and ctor functions for nodes
//
//------------------------------------------------------------------------------

#pragma once

#include <string>

#include "llvm/IR/Value.h"

namespace Node {

// node interface
struct INode {
  virtual int calc() = 0;
  virtual llvm::Value *codegen() = 0;
  virtual void dump() const = 0;
  virtual ~INode() = default;
};

// scope interface
struct IScope : public INode {
  virtual IScope *push() = 0;
  virtual IScope *resetScope() const = 0;
  virtual void addBranch(INode *Branch) = 0;
  virtual INode *access(const std::string &VarName) = 0;
  virtual INode *visible(const std::string &VarName) const = 0;
};

// operations
enum class Operation {
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

} // namespace Node

// ctor functions
Node::INode *makeValue(int V = 0);
Node::INode *makeOperand(Node::INode *Lhs, Node::Operation Op,
                         Node::INode *Rhs);
Node::INode *makeWhile(Node::INode *Op, Node::INode *Scope);
Node::INode *makeIf(Node::INode *Op, Node::INode *Scope);
Node::IScope *createScope();

extern Node::IScope *CurrentScope;
