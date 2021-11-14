/* ------------------------------------------------------------------------- **
 *
 * Source code for MIPT ILab
 * Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
 * Licensed after GNU GPL v3
 *
 * ------------------------------------------------------------------------- **
 *
 *  Grammar with shift/reduce, but correct
 *  EQL -> EQ; EQL | empty
 *  EQ -> E = E
 *  E -> number | E + number | E - number
 *
 * ------------------------------------------------------------------------- */

%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::NumDriver* driver}

%code requires
{
#include <algorithm>
#include <string>
#include <vector>

// forward decl of argument to parser
namespace yy { class NumDriver; }
}

%code
{
#include "numdriver.hpp"

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         NumDriver* driver);
}
}

%token
  EQUAL   "="
  MINUS   "-"
  PLUS    "+"
  SCOLON  ";"
  ERR
;

%token <int> NUMBER
%nterm <std::vector<int>> expr
%nterm <std::pair<std::vector<int>, std::vector<int>>> equals
%nterm <std::vector<std::pair<std::vector<int>, std::vector<int>>>> eqlist

%left '+' '-'

%start program

%%

program: eqlist               { driver->insert($1); }
;

eqlist: equals SCOLON eqlist  { $$ = $3; $$.push_back($1); }
      | equals SCOLON         { $$.push_back($1);          }
;

equals: expr EQUAL expr       { $$ = std::make_pair($1, $3); }
;

expr: NUMBER                  { $$.push_back($1); }
    | expr PLUS NUMBER        { $$ = $1; $$.push_back($3); }
    | expr MINUS NUMBER       { $$ = $1; $$.push_back(-$3); }
;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         NumDriver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&){}
}