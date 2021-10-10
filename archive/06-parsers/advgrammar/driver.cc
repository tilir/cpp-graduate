#include <iostream>

#include "numdriver.hpp"

// here we can return non-zero if lexing is not done inspite of EOF detected
int yyFlexLexer::yywrap() {
  return 1;
}

int main()
{
  FlexLexer* lexer = new yyFlexLexer;
  yy::NumDriver driver(lexer);
  driver.parse();
  driver.printout();
}