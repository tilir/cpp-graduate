%option c++

%{

#include "numgrammar.tab.hh"

%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]

%%

{WS}              /* skip blanks and tabs */
"+"               return yy::parser::token_type::PLUS;
"-"               return yy::parser::token_type::MINUS;
"="               return yy::parser::token_type::EQUAL;
";"               return yy::parser::token_type::SCOLON;
{DIGIT1}{DIGIT}*  return yy::parser::token_type::NUMBER;
.			            return yy::parser::token_type::ERR;

%%

// nothing