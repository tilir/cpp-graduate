%{

//------------------------------------------------------------------------------
//
// scanner.l -- lexical rules for ParaCL v2
//
//------------------------------------------------------------------------------

#include "parser.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ScanError(char const *, int lineno);

%}

id          [a-zA-Z][a-zA-Z_0-9]*
ws          [ \t]+
int         [0-9]+
%option yylineno

%%
"\n"        {  yyinlinePos = 0; }
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
               yylval.op = Ops::Equal;
               return RELOP;
            }
"!="        {
               BeginToken(yytext, &yyinlinePos);
               yylval.op = Ops::NotEqual;
               return RELOP;
            }
"<"         {
               BeginToken(yytext, &yyinlinePos);
               yylval.op = Ops::Less;
               return RELOP;
            }
">"         {
               BeginToken(yytext, &yyinlinePos);
               yylval.op = Ops::Greater;
               return RELOP;
            }
"<="        {
               BeginToken(yytext, &yyinlinePos);
               yylval.op = Ops::LessEq;
               return RELOP;
            }
">="        {
               BeginToken(yytext, &yyinlinePos);
               yylval.op = Ops::GreaterEq;
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
               ScanError("Unexpected smth", yylineno);
               return ILLEGAL;  
            }
%%

int yywrap() { return 1; }

void ScanError(char const * s, int lineno) {
  fprintf(stdout, "Scanner error: %s in line %d\n", s, lineno);
}
