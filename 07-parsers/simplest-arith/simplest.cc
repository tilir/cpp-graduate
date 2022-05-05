//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Driver for additions and multiplications grammar
//
//----------------------------------------------------------------------------

#include "driver.hpp"

// here we can return non-zero if lexing is not done inspite of EOF detected
int yyFlexLexer::yywrap() { return 1; }

int main() {
  FlexLexer *lexer = new yyFlexLexer;
  yy::Driver driver(lexer);
  driver.parse();
  delete lexer;
}
