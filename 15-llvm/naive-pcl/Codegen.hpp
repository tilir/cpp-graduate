//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Codegen.hpp -- code generation support
// main design flaw: global code generator
//
//------------------------------------------------------------------------------

#pragma once

#include <map>
#include <string>

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
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "INode.hpp"

struct CodeGen {
  llvm::LLVMContext Context_;
  llvm::IRBuilder<> *Builder_;
  llvm::Module *Module_;
  std::map<std::string, llvm::Value *> NamedValues_;

  // Function initialized in StartFunction, this is questionable design
  // We need to call EndCurrentFunction mandatory
  // Can we redesign it?
  llvm::Function *Function_ = nullptr;

  CodeGen(std::string Name)
      : Builder_(new llvm::IRBuilder<>(Context_)),
        Module_(new llvm::Module(Name, Context_)) {}

  // assumption: extern void Name() { ..... }
  void StartFunction(std::string Name);

  void EndCurrentFunction() { Builder_->CreateRetVoid(); }

  void SaveModule(std::string ModuleName);

  llvm::Value *AddDeclRead(std::string Varname) {
    auto *V = NamedValues_[Varname];
    assert(V);
    auto *Ty = V->getType()->getPointerElementType();
    return Builder_->CreateLoad(Ty, V, Varname.c_str());
  }

  llvm::Value *AddDeclWrite(std::string Varname, llvm::Value *Rhs) {
    auto *V = NamedValues_[Varname];
    assert(V);
    return Builder_->CreateStore(Rhs, V);
  }

  void AddDecl(std::string Varname) {
    auto &&BB = Function_->getEntryBlock();
    auto Ty = llvm::Type::getInt32Ty(Context_);
    llvm::IRBuilder<> TmpB(&BB, BB.begin());
    auto *Alloca = TmpB.CreateAlloca(Ty, 0, Varname.c_str());
    NamedValues_[Varname] = Alloca;
  }

  llvm::BasicBlock *StartIf(llvm::Value *CondV) {
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(Context_, "then");
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(Context_, "endif");
    Builder_->CreateCondBr(CondV, ThenBB, MergeBB);
    Function_->getBasicBlockList().push_back(ThenBB);
    Builder_->SetInsertPoint(ThenBB);
    return MergeBB;
  }

  void EndIf(llvm::BasicBlock *MergeBB) {
    // assume we are now in ThenBB
    Function_->getBasicBlockList().push_back(MergeBB);
    Builder_->CreateBr(MergeBB);
    Builder_->SetInsertPoint(MergeBB);
  }

  using WhileBlocksTy = std::pair<llvm::BasicBlock *, llvm::BasicBlock *>;

  WhileBlocksTy StartWhile(llvm::Value *CondV) {
    llvm::BasicBlock *BodyBB = llvm::BasicBlock::Create(Context_, "body");
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(Context_, "endwhile");
    Builder_->CreateCondBr(CondV, BodyBB, MergeBB);
    Function_->getBasicBlockList().push_back(BodyBB);
    Builder_->SetInsertPoint(BodyBB);
    return std::make_pair(BodyBB, MergeBB);
  }

  void EndWhile(llvm::Value *CondV, WhileBlocksTy BBs) {
    // assume we are now inside body
    Function_->getBasicBlockList().push_back(BBs.second);
    Builder_->CreateCondBr(CondV, BBs.first, BBs.second);
    Builder_->SetInsertPoint(BBs.second);
  }

  llvm::Value *AddOperation(Node::Operation Op, llvm::Value *LeftV,
                            llvm::Value *RightV);

  llvm::Type *getInt32Ty() { return llvm::Type::getInt32Ty(Context_); }

  llvm::Type *getVoidTy() { return llvm::Type::getVoidTy(Context_); }

  void createFnDecl(llvm::FunctionType *FT, std::string Name) {
    llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, Module_);
  }

  ~CodeGen() {
    delete Builder_;
    delete Module_;
  }
};

CodeGen *createCodeGen(std::string Name);

extern std::unique_ptr<CodeGen> GlobalGen;
