/* ------------------------------------------------------------------------- **
 *
 * Source code for MIPT ILab
 * Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
 * Licensed after GNU GPL v3
 *
 * ------------------------------------------------------------------------- **
 *
 *  Simplest, yet non-left-recursive grammar
 *  E -> T E'
 *  E' -> + T E' | %empty
 *  T -> F T'
 *  T' -> * F T' | %empty
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
%nterm <int> EPN
%nterm <int> TN
%nterm <int> TPN
%nterm <int> FN

%start program

%%

program: EN                   { std::cout << $1 << std::endl; }
;

EN: TN EPN                    { $$ = $1 + $2; }
;

EPN: PLUS TN EPN              { $$ = $2 + $3; }
   | %empty                   { $$ = 0; }
;

TN: FN TPN                    { $$ = $1 * $2; }
;

TPN: MULT FN TPN              { $$ = $2 * $3; }
   | %empty                   { $$ = 1; }
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
