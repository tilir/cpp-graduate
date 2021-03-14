//------------------------------------------------------------------------------
//
// driver.cpp -- main entry point
//
//------------------------------------------------------------------------------

#include <map>
#include <memory>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

#include "parser.hpp"

IScope *currentScope = nullptr;
llvm::LLVMContext *currentContext;
llvm::IRBuilder<> *currentBuilder;
llvm::Module *currentModule;
llvm::Function *currentFunction;

static int currentinlinePos = 0;

int main(int argc, char *argv[]) {
  FILE *f = fopen(argv[1], "r");
  if (f <= 0) {
    perror("Cannot open file");
    return 1;
  }
  yyin = f;
  currentScope = create_scope();
  currentContext = new llvm::LLVMContext;
  currentModule = new llvm::Module("pcl.module", *currentContext);
  currentBuilder = new llvm::IRBuilder<>(*currentContext);

  // for simplicity: single function
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getVoidTy(*currentContext), /* va args? */ false);

  currentFunction = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                           "pcl_start", currentModule);

  // basic block to start instruction insertion
  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(*currentContext, "entry", currentFunction);
  currentBuilder->SetInsertPoint(BB);

#ifdef DUMPALL
  llvm::errs() << "Before: ";
  currentModule->print(llvm::errs(), nullptr);
#endif

  yyparse();

#ifdef DUMPALL
  llvm::errs() << "After: ";
  currentModule->print(llvm::errs(), nullptr);
#endif

  fclose(f);
  delete currentScope;
  delete currentBuilder;
  delete currentContext;
  return 0;
}

void PrintError(char const *errorstring, ...) {
  static char errmsg[10000];
  va_list args;

  bool isNotNullPar = false;
  for (int i = 0; i < strlen(errorstring) - 1; ++i) {
    if (errorstring[i + 1] == '%' && errorstring[i] != '\\') {
      isNotNullPar = true;
      break;
    }
  }

  va_start(args, errorstring);
  vsprintf(errmsg, errorstring, args);
  va_end(args);
  if (!isNotNullPar)
    fprintf(stdout, "Error: %s - Line %d:c%d\n", errmsg, yylineno,
            currentinlinePos);
  else
    fprintf(stdout, "Error: %s\n", errmsg);

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
