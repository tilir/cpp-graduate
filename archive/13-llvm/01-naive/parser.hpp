//------------------------------------------------------------------------------
//
// parser.hpp -- bison related extern defines and retval struct
//
//------------------------------------------------------------------------------

#pragma once

#include "INode.hpp"
#include <cstdarg>
#include <cstring>
#include <sstream>
#include <string>

#ifndef CODEGEN
#define CODEGEN 1
#endif

extern IScope *currentScope;

#define YYERROR_VERBOSE 1

// function to run after parse
int wholeProgramAction();

struct yyRet {
  std::string name;
  int value;
  int linePos;
  int inLinePos;
  INode *treeNode;
  Ops op;
};

#define YYSTYPE yyRet

#include "compiler.cpp.h"

extern FILE *yyin;

int yylex();
int yyerror(char const *);
void PrintError(char const *s);
void BeginToken(char *, int *);

// line number diagnostics
extern int yylineno;

// for string position diagnostics
static int yyinlinePos;
