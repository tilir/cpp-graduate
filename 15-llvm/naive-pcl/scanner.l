%{

//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// scanner.l -- lexical rules for ParaCL v2
//
//------------------------------------------------------------------------------

#include <iostream>

#include "parser.hpp"

void ScanError(char const *, char const *, int lineno);

%}

id          [a-zA-Z][a-zA-Z_0-9]*
ws          [ \t]+
int         [0-9]+
%option yylineno

%%
"\n"        {  yyinlinePos = 0; }
"\r"        {}
{ws}        {  BeginToken(yytext, &yyinlinePos); }
"if"        {
               BeginToken(yytext, &yyinlinePos);
               return IF;
            }   
"while"     {
               BeginToken(yytext, &yyinlinePos);
               return WHILE;
            }
"print"     {
               BeginToken(yytext, &yyinlinePos);
               return OUTPUT;
            }
"+"         {  BeginToken(yytext, &yyinlinePos); return PLUS;  }
"-"         {  BeginToken(yytext, &yyinlinePos); return MINUS;  }
"*"         {  BeginToken(yytext, &yyinlinePos); return MUL;  }
"/"         {  BeginToken(yytext, &yyinlinePos); return DIV;  }
"%"         {  BeginToken(yytext, &yyinlinePos); return MOD;  }
"("         {  BeginToken(yytext, &yyinlinePos); return LPAR;  }
")"         {  BeginToken(yytext, &yyinlinePos); return RPAR;  }
"["         {  BeginToken(yytext, &yyinlinePos); return LBR;  }
"]"         {  BeginToken(yytext, &yyinlinePos); return RBR;  }
"{"         {  BeginToken(yytext, &yyinlinePos); return LCB;  }
"}"         {  BeginToken(yytext, &yyinlinePos); return RCB;  }
";"         {  BeginToken(yytext, &yyinlinePos); return SM;  }
"="         {  BeginToken(yytext, &yyinlinePos); return ASG;  }
"=="        {  BeginToken(yytext, &yyinlinePos);  
               yylval.op = Node::Operation::Equal;
               return RELOP;
            }
"!="        {
               BeginToken(yytext, &yyinlinePos);
               yylval.op = Node::Operation::NotEqual;
               return RELOP;
            }
"<"         {
               BeginToken(yytext, &yyinlinePos);
               yylval.op = Node::Operation::Less;
               return RELOP;
            }
">"         {
               BeginToken(yytext, &yyinlinePos);
               yylval.op = Node::Operation::Greater;
               return RELOP;
            }
"<="        {
               BeginToken(yytext, &yyinlinePos);
               yylval.op = Node::Operation::LessEq;
               return RELOP;
            }
">="        {
               BeginToken(yytext, &yyinlinePos);
               yylval.op = Node::Operation::GreaterEq;
               return RELOP;
            }
"||"        {  BeginToken(yytext, &yyinlinePos); return OR;  }
"&&"        {  BeginToken(yytext, &yyinlinePos); return AND;  }
"!"         {  BeginToken(yytext, &yyinlinePos); return NOT;  }
"?"         {  BeginToken(yytext, &yyinlinePos); return INPUT; }

{id}        {
               BeginToken(yytext, &yyinlinePos);
               yylval.name = yytext;
               return NAME;
            }
{int}       {
               BeginToken(yytext, &yyinlinePos);
               yylval.value = atoi(yytext);
               return VALUE;
            }
.           {  
               BeginToken(yytext, &yyinlinePos);
               ScanError("Unexpected smth", yytext, yylineno);
               return ILLEGAL;  
            }
%%

int yywrap() { return 1; }

void ScanError(char const * s, char const * yytext, int lineno) {
  std::cout << "Scanner error: " << s << " in line " << lineno << std::endl;
  std::cout << yytext << std::endl;
}
