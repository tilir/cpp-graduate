/* ------------------------------------------------------------------------- **
 *
 * Source code for MIPT ILab
 * Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
 * Licensed after GNU GPL v3
 *
 * ------------------------------------------------------------------------- **
 *
 *  Simplest, yet left-recursive grammar
 *  E -> E + T | T
 *  T -> T * F | F
 *  F -> ( E ) | number
 *
 * ------------------------------------------------------------------------- */

%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::Driver* driver}

%code requires
{
#include <iostream>
#include <string>

// forward decl of argument to parser
namespace yy { class Driver; }
}

%code
{
#include "driver.hpp"

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         Driver* driver);
}
}

%token
  PLUS   "+"
  MULT   "*"
  LBRAC  "("
  RBRAC  ")"
  ERR
;

%token <int> NUMBER
%nterm <int> EN
%nterm <int> FN
%nterm <int> TN

%start program

%%

program: EN                   { std::cout << $1 << std::endl; }
;

EN: EN PLUS TN                { $$ = $1 + $3; }
  | TN                        { $$ = $1; }
;

TN: TN MULT FN                { $$ = $1 * $3; }
  | FN                        { $$ = $1; }
;

FN: LBRAC EN RBRAC            { $$ = $2; }
  | NUMBER                    { $$ = $1; }
;


%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         Driver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&){}
}
