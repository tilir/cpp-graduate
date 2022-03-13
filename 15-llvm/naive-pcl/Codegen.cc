//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// Codegen.cc -- code generation support implementation
//
//------------------------------------------------------------------------------

#include <iostream>
#include <stdexcept>
#include <string>

#include "Codegen.hpp"
#include "INode.hpp"

void CodeGen::SaveModule(std::string ModuleName) {
  std::error_code EC;
  llvm::raw_fd_ostream Outfile{ModuleName, EC};
  if (EC)
    llvm::errs() << EC.message().c_str() << "\n";

  Module_->print(Outfile, nullptr);
  Outfile.close();

  if (Outfile.has_error()) {
    std::cerr << Outfile.error().message() << std::endl;
    throw std::runtime_error("Error printing to file: ");
  }
}

void CodeGen::StartFunction(std::string Name) {
  // single __pcl_start function for void module
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getVoidTy(Context_), /* va args? */ false);

  Function_ = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name,
                                     Module_);

  // basic block to start instruction insertion
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(Context_, "entry", Function_);
  Builder_->SetInsertPoint(BB);
}

llvm::Value *CodeGen::AddOperation(Node::Operation Op, llvm::Value *LeftV,
                                   llvm::Value *RightV) {
  switch (Op) {
  case Node::Operation::Plus:
    if (LeftV == nullptr)
      LeftV = llvm::ConstantInt::get(getInt32Ty(), 0);
    return Builder_->CreateAdd(LeftV, RightV);
  case Node::Operation::Minus:
    if (LeftV == nullptr)
      LeftV = llvm::ConstantInt::get(getInt32Ty(), 0);
    return Builder_->CreateSub(LeftV, RightV);
  case Node::Operation::Greater:
    return Builder_->CreateICmpSGT(LeftV, RightV);
  case Node::Operation::Less:
    return Builder_->CreateICmpSLT(LeftV, RightV);
  case Node::Operation::GreaterEq:
    return Builder_->CreateICmpSGE(LeftV, RightV);
  case Node::Operation::LessEq:
    return Builder_->CreateICmpSLE(LeftV, RightV);
  case Node::Operation::StdOut: {
    auto *CalleeF = Module_->getFunction("__pcl_print");
    assert(CalleeF && "Driver shall create decl for __pcl_print");
    assert(RightV && "Print required non-null right arg");
    llvm::Value *ArgsV[] = {RightV};
    return Builder_->CreateCall(CalleeF, ArgsV);
  }
  case Node::Operation::StdIn: {
    auto *CalleeF = Module_->getFunction("__pcl_scan");
    assert(CalleeF && "Driver shall create decl for __pcl_scan");
    return Builder_->CreateCall(CalleeF);
  }
  case Node::Operation::Equal:
    return Builder_->CreateICmpEQ(LeftV, RightV);
  case Node::Operation::NotEqual:
    return Builder_->CreateICmpNE(LeftV, RightV);
  case Node::Operation::Not:
    return Builder_->CreateNot(RightV);
  case Node::Operation::And:
    return Builder_->CreateAnd(LeftV, RightV);
  case Node::Operation::Or:
    return Builder_->CreateOr(LeftV, RightV);
  case Node::Operation::Div:
    return Builder_->CreateSDiv(LeftV, RightV);
  case Node::Operation::Mul:
    return Builder_->CreateMul(LeftV, RightV);
  case Node::Operation::Mod:
    return Builder_->CreateSRem(LeftV, RightV);
  default:
    break;
  }

  // to implement
  throw std::runtime_error("Node::Operation not implemented");
}

CodeGen *createCodeGen(std::string Name) {
  CodeGen *Gen = new CodeGen(Name);

  // prototype for print and scan functions
  llvm::Type *Tys[] = {Gen->getInt32Ty()};
  llvm::FunctionType *FTPrint =
      llvm::FunctionType::get(Gen->getVoidTy(), Tys, /* va args? */ false);
  llvm::FunctionType *FTScan =
      llvm::FunctionType::get(Gen->getInt32Ty(), /* va args? */ false);

  // creating decls for modules
  Gen->createFnDecl(FTPrint, "__pcl_print");
  Gen->createFnDecl(FTScan, "__pcl_scan");
  return Gen;
}
