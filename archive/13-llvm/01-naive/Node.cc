//------------------------------------------------------------------------------
//
// Node.cpp -- concrete node types implementation
//
//------------------------------------------------------------------------------

#include "llvm/ADT/APInt.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "Node.hpp"
#include "Symtab.hpp"

Symtab globalTable;
std::map<std::string, llvm::Value *> NamedValues;

extern llvm::LLVMContext *currentContext;
extern llvm::IRBuilder<> *currentBuilder;
extern llvm::Function *currentFunction;
extern llvm::Module *currentModule;

static llvm::AllocaInst *CreateEntryBlockAlloca(const std::string &varname) {

  llvm::IRBuilder<> TmpB(&currentFunction->getEntryBlock(),
                         currentFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(llvm::Type::getInt32Ty(*currentContext), 0,
                           varname.c_str());
}

INode *make_value(int v) { return new Value{v}; }
INode *make_op(INode *l, Ops o, INode *r) { return new Op{l, o, r}; }
INode *make_while(INode *o, INode *s) { return new While{o, s}; }
INode *make_if(INode *o, INode *s) { return new If{o, s}; }
IScope *create_scope() { return new Scope{nullptr}; }

// NUMBER
RType Value::calc() { return val; }
llvm::Value *Value::codegen() {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*currentContext), val);
}
void Value::dump() const { std::cout << "Node Value: " << val << std::endl; }

RType Decl::calc() { return val; }
llvm::Value *Decl::codegen() {
  llvm::Value *V = NamedValues[varname];
  assert(V);
  return currentBuilder->CreateLoad(V, varname.c_str());
}
llvm::Value *Decl::assign(llvm::Value *RHS) {
  llvm::Value *V = NamedValues[varname];
  assert(V);
  return currentBuilder->CreateStore(RHS, V);
}

void Decl::dump() const { std::cout << "Node Decl: " << val << std::endl; }
void Decl::SetValue(int Val) { val = Val; }

// SCOPE

RType Scope::calc() {
  for (auto x : branches)
    x->calc();

  return 0;
}
llvm::Value *Scope::codegen() {
  for (auto x : branches)
    x->codegen();
  return nullptr;
}
void Scope::dump() const {
  std::cout << "Node Scope: " << std::endl;
  for (auto const x : branches)
    x->dump();
}
IScope *Scope::resetScope() const { return prev_scope; }
void Scope::addBranch(INode *branch) { branches.push_back(branch); }

INode *Scope::access(std::string const &var_name) {
  INode *t = visible(var_name);
  if (t)
    return t;

  INode *d = new Decl{var_name};

#if (CODEGEN == 1)
  auto *Alloca = CreateEntryBlockAlloca(var_name);
  NamedValues[var_name] = Alloca;
#endif

  globalTable.add(this, var_name, d);
  return d;
}

INode *Scope::visible(std::string const &var_name) {
  INode *f = globalTable.exists(this, var_name);
  if (f)
    return f;
  if (prev_scope)
    f = prev_scope->visible(var_name);
  return f;
}

Scope::~Scope() {
  for (auto x : branches)
    delete x;

  globalTable.free(this);
}

RType Op::calc() {
  int val = 0;
  switch (op) {
  case Ops::Plus:
    if (left == nullptr)
      return 0 + right->calc();
    return left->calc() + right->calc();
  case Ops::Minus:
    if (left == nullptr)
      return 0 - right->calc();
    return left->calc() - right->calc();
  case Ops::Greater:
    return left->calc() > right->calc();
  case Ops::Less:
    return left->calc() < right->calc();
  case Ops::GreaterEq:
    return left->calc() >= right->calc();
  case Ops::LessEq:
    return left->calc() <= right->calc();
  case Ops::Assign:
    val = right->calc();
    static_cast<Decl *>(left)->SetValue(val);
    return val;
  case Ops::StdOut:
    std::cout << right->calc() << std::endl;
    break;
  case Ops::StdIn:
    std::cin >> val;
    return val;
  case Ops::Equal:
    return (left->calc() == right->calc());
  case Ops::NotEqual:
    return (left->calc() != right->calc());
  case Ops::Not:
    return !static_cast<bool>(right->calc());
  case Ops::And:
    return left->calc() && right->calc();
  case Ops::Or:
    return left->calc() || right->calc();
  case Ops::Div:
    val = right->calc();
    if (!val) {
      std::cout << "Division by zero!" << std::endl;
      exit(1);
    }
    return left->calc() / right->calc();
  case Ops::Mul:
    return left->calc() * right->calc();
  case Ops::Mod:
    return left->calc() % right->calc();
  default:
    return 999;
  }

  return 998;
}

llvm::Value *Op::codegen() {
  llvm::Value *LeftV = nullptr;
  llvm::Value *RightV = nullptr;
  if (left)
    LeftV = left->codegen();
  if (right)
    RightV = right->codegen();
  switch (op) {
  case Ops::Plus:
    if (LeftV == nullptr)
      LeftV =
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(*currentContext), 0);
    return currentBuilder->CreateAdd(LeftV, RightV);
  case Ops::Minus:
    if (LeftV == nullptr)
      LeftV =
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(*currentContext), 0);
    return currentBuilder->CreateSub(LeftV, RightV);
  case Ops::Greater:
    return currentBuilder->CreateICmpSGT(LeftV, RightV);
  case Ops::Less:
    return currentBuilder->CreateICmpSLT(LeftV, RightV);
  case Ops::GreaterEq:
    return currentBuilder->CreateICmpSGE(LeftV, RightV);
  case Ops::LessEq:
    return currentBuilder->CreateICmpSLE(LeftV, RightV);
  case Ops::Assign:
    return static_cast<Decl *>(left)->assign(RightV);
  case Ops::StdOut: {
    auto *CalleeF = currentModule->getFunction("__pcl_print");
    assert(CalleeF && "Driver shall create decl for __pcl_print");
    assert(RightV && "Print required non-null right arg");
    llvm::Value *ArgsV[] = {RightV};
    return currentBuilder->CreateCall(CalleeF, ArgsV);
  }
  case Ops::StdIn: {
    auto *CalleeF = currentModule->getFunction("__pcl_scan");
    assert(CalleeF && "Driver shall create decl for __pcl_scan");
    return currentBuilder->CreateCall(CalleeF);
  }
  case Ops::Equal:
    return currentBuilder->CreateICmpEQ(LeftV, RightV);
  case Ops::NotEqual:
    return currentBuilder->CreateICmpNE(LeftV, RightV);
  case Ops::Not:
    return currentBuilder->CreateNot(RightV);
  case Ops::And:
    return currentBuilder->CreateAnd(LeftV, RightV);
  case Ops::Or:
    return currentBuilder->CreateOr(LeftV, RightV);
  case Ops::Div:
    return currentBuilder->CreateSDiv(LeftV, RightV);
  case Ops::Mul:
    return currentBuilder->CreateMul(LeftV, RightV);
  case Ops::Mod:
    return currentBuilder->CreateSRem(LeftV, RightV);
  default:
    break;
  }

  // to implement
  return nullptr;
}

void Op::dump() const {
  std::cout << "Node Op: ";
  int val = 0;
  switch (op) {
  case Ops::Plus:
    std::cout << "Plus" << std::endl;
    break;
  case Ops::Minus:
    std::cout << "Minus" << std::endl;
    break;
  case Ops::Greater:
    std::cout << "Greater" << std::endl;
    break;
  case Ops::Less:
    std::cout << "Less" << std::endl;
    break;
  case Ops::Assign:
    std::cout << "Assign" << std::endl;
    break;
  case Ops::StdOut:
    std::cout << "StdOut" << std::endl;
    break;
  case Ops::StdIn:
    std::cout << "StdIn" << std::endl;
    break;
  case Ops::Equal:
    std::cout << "Equal" << std::endl;
    break;
  default:
    std::cout << "Unknown" << std::endl;
  }
  if (left != nullptr)
    left->dump();
  right->dump();
}

Op::~Op() {
  if (left && typeid(Decl) != typeid(*left))
    delete left;
  if (right && typeid(Decl) != typeid(*right))
    delete right;
}

RType While::calc() {
  while (op->calc())
    scope->calc();

  return 0;
}

llvm::Value *While::codegen() {
  // to implement
  return nullptr;
}

void While::dump() const {
  std::cout << "Node While " << std::endl;
  scope->dump();
}
While::~While() {
  if (op && typeid(Decl) != typeid(*op))
    delete op;
  delete scope;
}

RType If::calc() {
  if (op->calc())
    scope->calc();

  return 0;
}

llvm::Value *If::codegen() {
  // to implement
  return nullptr;
}

void If::dump() const {
  std::cout << "Node If " << std::endl;
  scope->dump();
}
If::~If() {
  if (op && typeid(Decl) != typeid(*op))
    delete op;
  delete scope;
}
