/* ------------------------------------------------------------------------- **
 *
 * Source code for MIPT ILab
 * Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
 * Licensed after GNU GPL v3
 *
 * ------------------------------------------------------------------------- **
 *
 *  Simplest, yet ambigouos grammar
 *  E -> E + E | E * E | ( E ) | number
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

%start program

%%

program: EN                   { std::cout << $1 << std::endl; }
;

EN: EN PLUS EN                { $$ = $1 + $3; }
  | EN MULT EN                { $$ = $1 * $3; }
  | LBRAC EN RBRAC            { $$ = $2; }
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
