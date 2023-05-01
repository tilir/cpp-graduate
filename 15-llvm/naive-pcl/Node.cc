//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Node.cpp -- concrete node types implementation
//
//------------------------------------------------------------------------------

#include <map>
#include <stdexcept>
#include <string>

#include "Codegen.hpp"
#include "Node.hpp"
#include "Symtab.hpp"

Symtab GlobalTable;

Node::INode *makeValue(int V) { return new Node::Value{V}; }
Node::INode *makeOperand(Node::INode *Lhs, Node::Operation Op,
                         Node::INode *Rhs) {
  return new Node::Operand{Lhs, Op, Rhs};
}
Node::INode *makeWhile(Node::INode *Op, Node::INode *Scope) {
  return new Node::While{Op, Scope};
}
Node::INode *makeIf(Node::INode *Op, Node::INode *Scope) {
  return new Node::If{Op, Scope};
}
Node::IScope *createScope() { return new Node::Scope{nullptr}; }

namespace Node {

// SCOPE

INode *Scope::access(std::string const &VarName) {
  INode *Existing = visible(VarName);
  if (Existing)
    return Existing;

  INode *NewDecl = new Decl{VarName};

#if (CODEGEN == 1)
  GlobalGen->AddDecl(VarName);
#endif

  GlobalTable.add(this, VarName, NewDecl);
  return NewDecl;
}

INode *Scope::visible(std::string const &VarName) const {
  INode *F = GlobalTable.exists(this, VarName);
  if (F)
    return F;
  if (PrevScope_)
    F = PrevScope_->visible(VarName);
  return F;
}

Scope::~Scope() {
  for (auto *B : Branches_)
    delete B;

  GlobalTable.free(this);
}

int Operand::calc() {
  int Val = 0;
  switch (Op_) {
  case Operation::Plus:
    if (Left_ == nullptr)
      return Right_->calc();
    return Left_->calc() + Right_->calc();
  case Operation::Minus:
    if (Left_ == nullptr)
      return -Right_->calc();
    return Left_->calc() - Right_->calc();
  case Operation::Greater:
    return Left_->calc() > Right_->calc();
  case Operation::Less:
    return Left_->calc() < Right_->calc();
  case Operation::GreaterEq:
    return Left_->calc() >= Right_->calc();
  case Operation::LessEq:
    return Left_->calc() <= Right_->calc();
  case Operation::Assign:
    Val = Right_->calc();
    static_cast<Decl *>(Left_)->SetValue(Val);
    return Val;
  case Operation::StdOut:
    std::cout << Right_->calc() << std::endl;
    break;
  case Operation::StdIn:
    std::cin >> Val;
    return Val;
  case Operation::Equal:
    return (Left_->calc() == Right_->calc());
  case Operation::NotEqual:
    return (Left_->calc() != Right_->calc());
  case Operation::Not:
    return !static_cast<bool>(Right_->calc());
  case Operation::And:
    return Left_->calc() && Right_->calc();
  case Operation::Or:
    return Left_->calc() || Right_->calc();
  case Operation::Div:
    Val = Right_->calc();
    if (!Val)
      throw std::runtime_error("Division by zero");
    return Left_->calc() / Right_->calc();
  case Operation::Mul:
    return Left_->calc() * Right_->calc();
  case Operation::Mod:
    return Left_->calc() % Right_->calc();
  default:
    throw std::runtime_error("Operation not supported");
  }
  return Val;
}

void Operand::dump() const {
  std::cout << "Node Operand: ";
  switch (Op_) {
  case Operation::Plus:
    std::cout << "Plus" << std::endl;
    break;
  case Operation::Minus:
    std::cout << "Minus" << std::endl;
    break;
  case Operation::Greater:
    std::cout << "Greater" << std::endl;
    break;
  case Operation::Less:
    std::cout << "Less" << std::endl;
    break;
  case Operation::Assign:
    std::cout << "Assign" << std::endl;
    break;
  case Operation::StdOut:
    std::cout << "StdOut" << std::endl;
    break;
  case Operation::StdIn:
    std::cout << "StdIn" << std::endl;
    break;
  case Operation::Equal:
    std::cout << "Equal" << std::endl;
    break;
  default:
    std::cout << "Unknown" << std::endl;
  }
  if (Left_ != nullptr)
    Left_->dump();
  Right_->dump();
}

// design flaw: dtor is recursive
Operand::~Operand() {
  if (Left_ && typeid(Decl) != typeid(*Left_))
    delete Left_;
  if (Right_ && typeid(Decl) != typeid(*Right_))
    delete Right_;
}

} // namespace Node
