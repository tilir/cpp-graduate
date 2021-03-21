//------------------------------------------------------------------------------
//
// driver.cpp -- main entry point
//
//------------------------------------------------------------------------------

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"

#include "parser.hpp"

IScope *currentScope = nullptr;

llvm::LLVMContext *currentContext;
llvm::IRBuilder<> *currentBuilder;
llvm::Module *currentModule;
llvm::Function *currentFunction;

static int currentinlinePos = 0;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " file.pcl" << std::endl;
    return 0;
  }

  FILE *f = fopen(argv[1], "r");
  if (f <= 0) {
    perror("Cannot open file");
    return 1;
  }
  yyin = f;
  currentScope = create_scope();

#if (CODEGEN == 1)
  currentContext = new llvm::LLVMContext;
  currentModule = new llvm::Module("pcl.module", *currentContext);
  currentBuilder = new llvm::IRBuilder<>(*currentContext);

  // prototype for print function
  llvm::Type *Tys[] = {llvm::Type::getInt32Ty(*currentContext)};

  llvm::FunctionType *FTPrint = llvm::FunctionType::get(
      llvm::Type::getVoidTy(*currentContext), Tys, /* va args? */ false);

  llvm::Function::Create(FTPrint, llvm::Function::ExternalLinkage,
                         "__pcl_print", currentModule);

  // prototype for scan function
  llvm::FunctionType *FTScan = llvm::FunctionType::get(
      llvm::Type::getInt32Ty(*currentContext), /* va args? */ false);

  llvm::Function::Create(FTScan, llvm::Function::ExternalLinkage, "__pcl_scan",
                         currentModule);

  // single __pcl_start function for void module
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getVoidTy(*currentContext), /* va args? */ false);

  currentFunction = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                           "__pcl_start", currentModule);

  // basic block to start instruction insertion
  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(*currentContext, "entry", currentFunction);
  currentBuilder->SetInsertPoint(BB);
#endif

  yyparse();

#if (CODEGEN == 1)
  // very bad assumption that currentBuilder is basically the same
  // will not hold for multiple functions, etc

  currentBuilder->CreateRetVoid();
#endif

#if (CODEGEN == 1)
  std::ostringstream s;
  s << std::filesystem::path(argv[1]).filename().string() << ".ll";

  std::cout << "Saving module to: " << s.str() << "\n";

  std::error_code EC;
  llvm::raw_fd_ostream outfile{s.str(), EC};
  if (EC) {
    llvm::errs() << EC.message().c_str() << "\n";
  }

  currentModule->print(outfile, nullptr);
  outfile.close();

  if (outfile.has_error()) {
    llvm::errs() << "Error printing to file: " << outfile.error().message()
                 << "\n";
  }
#endif

  fclose(f);

#if (CODEGEN == 1)
  delete currentBuilder;
  delete currentContext;
#endif

  delete currentScope;
  return 0;
}

void PrintError(char const *errmsg) {
  std::cerr << "Error: " << errmsg << " - Line " << yylineno << ", Column "
            << currentinlinePos << std::endl;
  exit(1);
}

void BeginToken(char *t, int *yyinlinePos) {
  yylloc.first_line = yylineno;
  yylloc.first_column = *yyinlinePos;
  yylloc.last_line = yylineno;
  *yyinlinePos += strlen(t);
  yylloc.last_column = *yyinlinePos;

  currentinlinePos = *yyinlinePos;
}
