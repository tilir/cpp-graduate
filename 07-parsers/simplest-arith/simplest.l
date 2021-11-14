%option c++

%{

#include "simplest.tab.hh"

%}

WS      [ \t\n\v]+
DIGIT   [0-9]

%%

{WS}              /* skip blanks and tabs */
"+"               return yy::parser::token_type::PLUS;
"*"               return yy::parser::token_type::MULT;
"("               return yy::parser::token_type::LBRAC;
")"               return yy::parser::token_type::RBRAC;
{DIGIT}{DIGIT}*   return yy::parser::token_type::NUMBER;
.			            return yy::parser::token_type::ERR;

%%

// nothing