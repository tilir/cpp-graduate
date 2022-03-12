%{

//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//------------------------------------------------------------------------------
//
// compiler.y -- grammar rules for ParaCL v2
//
// Of course global CurrentScope is design flaw here.
//
//------------------------------------------------------------------------------

#include "parser.hpp"

%}

%defines

%token NAME VALUE
%token WHILE IF
%token SM LBR RBR LPAR RPAR LCB RCB ILLEGAL
%token OUTPUT
%right ASG
%nonassoc RELOP
%left INPUT
%left OR
%left AND
%left NOT
%left PLUS MINUS
%left MUL DIV MOD
%nonassoc UMINUS

%%

program:      stms                    { wholeProgramAction(); }

scope:        open_sc stms close_sc   { $$ = $3; }

open_sc:      LCB                     { CurrentScope = CurrentScope->push(); }

close_sc:     RCB                     {
                                        $$.treeNode = CurrentScope;
                                        CurrentScope = CurrentScope->resetScope();
                                      }
            
stms:         stm                     { CurrentScope->addBranch($1.treeNode); }
            | stms stm                { CurrentScope->addBranch($2.treeNode); }
            | stms scope              { CurrentScope->addBranch($2.treeNode); }

stm:          oper                    { $$ = $1; }

oper:         assign                  { $$ = $1; }
            | if                      { 
                                        CurrentScope = CurrentScope->resetScope();
                                        $$ = $1;
                                      }
            | while                   { 
                                        CurrentScope = CurrentScope->resetScope();
                                        $$ = $1;
                                      }
            | output                  { $$ = $1; }

assign:       lval ASG expr SM        { $$.treeNode = makeOperand($1.treeNode, Node::Operation::Assign, $3.treeNode); }

lval:         NAME                    { $$.treeNode = CurrentScope->access($1.name); }
              
expr:         expr PLUS expr          { $$.treeNode = makeOperand($1.treeNode, Node::Operation::Plus, $3.treeNode); }
            | expr MINUS expr         { $$.treeNode = makeOperand($1.treeNode, Node::Operation::Minus, $3.treeNode); }
            | expr MUL expr           { $$.treeNode = makeOperand($1.treeNode, Node::Operation::Mul, $3.treeNode); }
            | expr DIV expr           { $$.treeNode = makeOperand($1.treeNode, Node::Operation::Div, $3.treeNode); }
            | expr MOD expr           { $$.treeNode = makeOperand($1.treeNode, Node::Operation::Mod, $3.treeNode); }
            | MINUS expr %prec UMINUS { $$.treeNode = makeOperand(makeValue(), Node::Operation::Minus, $2.treeNode); }
            | NAME                    { 
                                        $$.treeNode = CurrentScope->visible($1.name);
                                        if (!$$.treeNode) {
                                          YYLTYPE * info = &@1;
                                          PrintError("Using undeclared variable");
                                        }
                                      }
            | VALUE                   { $$.treeNode = makeValue($1.value); }
            | INPUT                   { $$.treeNode = makeOperand(nullptr, Node::Operation::StdIn, nullptr); }
            | LPAR expr RPAR          { $$.treeNode = $2.treeNode; }

if:           if_head stm             { 
                                        CurrentScope->addBranch($2.treeNode);
                                        $$.treeNode = makeIf($1.treeNode, CurrentScope);
                                      }
            | if_head scope           { 
                                        CurrentScope->addBranch($2.treeNode);
                                        $$.treeNode = makeIf($1.treeNode, CurrentScope);
                                      }

if_head:      IF LPAR lexpr RPAR      {
                                        CurrentScope = CurrentScope->push();
                                        $$ = $3;
                                      }

lexpr:        expr                    { $$ = $1; }
            | expr RELOP expr         { $$.treeNode = makeOperand($1.treeNode, $2.op, $3.treeNode); }
            | lexpr OR lexpr          { $$.treeNode = makeOperand($1.treeNode, Node::Operation::Or, $3.treeNode); }
            | lexpr AND lexpr         { $$.treeNode = makeOperand($1.treeNode, Node::Operation::And, $3.treeNode); }
            | NOT lexpr               { $$.treeNode = makeOperand(nullptr, Node::Operation::Not, $2.treeNode); }
            | LPAR lexpr RPAR         { $$ = $2; }

while:        while_head stm          { 
                                        CurrentScope->addBranch($2.treeNode);
                                        $$.treeNode = makeWhile($1.treeNode, CurrentScope);
                                      }
            | while_head scope        { 
                                        CurrentScope->addBranch($2.treeNode);
                                        $$.treeNode = makeWhile($1.treeNode, CurrentScope);
                                      }

while_head:   WHILE LPAR lexpr RPAR   {
                                        CurrentScope = CurrentScope->push();
                                        $$ = $3;
                                      }

output:       OUTPUT expr SM          { $$.treeNode = makeOperand(nullptr, Node::Operation::StdOut, $2.treeNode); }

%%
    
int yyerror(const char *s) {
  PrintError(s);
  return 0;
}

void wholeProgramAction() {
#if (CODEGEN == 1)
  CurrentScope->codegen();
#else
  CurrentScope->calc();
#endif  
}
