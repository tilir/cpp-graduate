//------------------------------------------------------------------------------
//
// Node.hpp -- concrete node types
//
//------------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#include "INode.hpp"

using RType = int;

// Integer value
class Value final : public INode {
  int val;

  // INode interface
public:
  RType calc() override;
  llvm::Value *codegen() override;
  void dump() const override;

public:
  Value(int v) : val(v) {}
};

// Declaration
class Decl final : public INode {
  int val;
  std::string varname;

  // INode interface
public:
  Decl(const std::string &n) : varname(n) {}
  RType calc() override;
  llvm::Value *codegen() override;
  llvm::Value *assign(llvm::Value *RHS);
  void dump() const override;

public:
  Decl() = default;
  void SetValue(int Val);
};

// Scope
class Scope final : public IScope {
  std::vector<INode *> branches;
  IScope *prev_scope;

  // INode interface
public:
  RType calc() override;
  llvm::Value *codegen() override;
  void dump() const override;

  // IScope interface
public:
  IScope *push() { return new Scope(this); }
  IScope *resetScope() const override;
  void addBranch(INode *branch) override;
  INode *access(std::string const &var_name);
  INode *visible(std::string const &var_name);

public:
  Scope(Scope *prev) : prev_scope(prev) {}
  ~Scope();
};

// Operand
class Op final : public INode {
  INode *right;
  INode *left;
  Ops op;

  // INode interface
public:
  RType calc() override;
  llvm::Value *codegen() override;
  void dump() const override;

public:
  Op(INode *l, Ops o, INode *r) : left(l), right(r), op(o) {}
  ~Op();
};

// While loop node
class While final : public INode {
  INode *op = nullptr;
  INode *scope = nullptr;

  // INode interface
public:
  RType calc() override;
  llvm::Value *codegen() override;
  void dump() const override;

public:
  While(INode *o, INode *s) : op(o), scope(s) {}
  ~While();
};

// If node
class If final : public INode {
  INode *op;
  INode *scope;

  // INode interface
public:
  RType calc() override;
  llvm::Value *codegen() override;
  void dump() const override;

public:
  If(INode *o, INode *s) : op(o), scope(s) {}
  ~If();
};
