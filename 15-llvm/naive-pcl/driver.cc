//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
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

#include "Codegen.hpp"
#include "parser.hpp"

Node::IScope *CurrentScope = nullptr;
std::unique_ptr<CodeGen> GlobalGen = nullptr;

static int CurrentInlinePos = 0;

FILE *OpenFile(const char *Name) {
  FILE *F = fopen(Name, "r");
  if (!F) {
    perror("Cannot open file");
    throw std::runtime_error("No input file exists");
  }
  return F;
}

int main(int argc, char *argv[]) try {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " file.pcl" << std::endl;
    return 0;
  }

  auto fdeleter = [](FILE *f) { fclose(f); };

  std::unique_ptr<FILE, decltype(fdeleter)> F(OpenFile(argv[1]), fdeleter);
  yyin = F.get();
  std::unique_ptr<Node::IScope> CurrentScopeOwner{createScope()};
  CurrentScope = CurrentScopeOwner.get();

#if (CODEGEN == 1)
  GlobalGen.reset(createCodeGen("pcl.module"));
  GlobalGen->StartFunction("__pcl_start");
#endif

  yyparse();

#if (CODEGEN == 1)
  // bad assumption that function is single
  GlobalGen->EndCurrentFunction();

  // save module to file
  auto ModuleName = std::filesystem::path(argv[1]).filename().string() + ".ll";
  std::cout << "Saving module to: " << ModuleName << "\n";
  GlobalGen->SaveModule(ModuleName);
#endif
} catch (const std::exception &e) {
  std::cerr << "Exception: " << e.what() << std::endl;
} catch (...) {
  std::cerr << "Exception unknown\n";
}

void PrintError(char const *Errmsg) {
  std::cerr << "Error: " << Errmsg << " - Line " << yylineno << ", Column "
            << CurrentInlinePos << std::endl;
  throw std::runtime_error("parse error");
}

void BeginToken(char *t, int *yyinlinePos) {
  yylloc.first_line = yylineno;
  yylloc.first_column = *yyinlinePos;
  yylloc.last_line = yylineno;
  *yyinlinePos += strlen(t);
  yylloc.last_column = *yyinlinePos;
  CurrentInlinePos = *yyinlinePos;
}
