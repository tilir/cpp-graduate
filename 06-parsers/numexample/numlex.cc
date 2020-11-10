#include <iostream>

#include <FlexLexer.h>

// here we can return non-zero if lexing is not done inspite of EOF detected
int yyFlexLexer::yywrap() {
  return 1;
}

int main()
{
  FlexLexer* lexer = new yyFlexLexer;
  while(lexer->yylex() != 0) {
    // do nothing for now, all is in rules
  }
  
  delete lexer;
}