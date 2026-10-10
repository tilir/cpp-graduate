//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Object-oriented lexer driver program
//
//----------------------------------------------------------------------------

#include "numlexer.hpp"
#include <iostream>
#include <memory>

int yyFlexLexer::yywrap() { return 1; }

int main() {
  auto lexer = std::make_unique<NumLexer>();
  while (lexer->yylex() != 0) {
    lexer->print_current();
  }
}