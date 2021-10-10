%option yyclass="NumLexer"
%option c++

%{

#include "numlexer.hpp"

%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]

%%

{WS}              /* skip blanks and tabs */
"+"               return process_plus();
"-"               return process_minus();
"="               return process_eq();
{DIGIT1}{DIGIT}*  return process_digit();
.                 return process_unknown();

%%

// nothing