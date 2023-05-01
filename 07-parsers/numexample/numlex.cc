//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Simplest lexer driver program
//
//----------------------------------------------------------------------------

#include <iostream>

#include <FlexLexer.h>

// here we can return non-zero if lexing is not done inspite of EOF detected
int yyFlexLexer::yywrap() { return 1; }

int main() {
  FlexLexer *lexer = new yyFlexLexer;
  while (lexer->yylex() != 0) {
    // do nothing for now, all is in rules
  }

  delete lexer;
}