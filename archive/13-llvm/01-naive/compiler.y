%{

//------------------------------------------------------------------------------
//
// compiler.y -- grammar rules for ParaCL v2
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

open_sc:      LCB                     { currentScope = currentScope->push(); }

close_sc:     RCB                     {
                                        $$.treeNode = currentScope;
                                        currentScope = currentScope->resetScope();
                                      }
            
stms:         stm                     { currentScope->addBranch($1.treeNode); }
            | stms stm                { currentScope->addBranch($2.treeNode); }
            | stms scope              { currentScope->addBranch($2.treeNode); }

stm:          oper                    { $$ = $1; }

oper:         assign                  { $$ = $1; }
            | if                      { 
                                        currentScope = currentScope->resetScope();
                                        $$ = $1;
                                      }
            | while                   { 
                                        currentScope = currentScope->resetScope();
                                        $$ = $1;
                                      }
            | output                  { $$ = $1; }

assign:       lval ASG expr SM        { $$.treeNode = make_op($1.treeNode, Ops::Assign, $3.treeNode); }

lval:         NAME                    { $$.treeNode = currentScope->access($1.name); }
              
expr:         expr PLUS expr          { $$.treeNode = make_op($1.treeNode, Ops::Plus, $3.treeNode); }
            | expr MINUS expr         { $$.treeNode = make_op($1.treeNode, Ops::Minus, $3.treeNode); }
            | expr MUL expr           { $$.treeNode = make_op($1.treeNode, Ops::Mul, $3.treeNode); }
            | expr DIV expr           { $$.treeNode = make_op($1.treeNode, Ops::Div, $3.treeNode); }
            | expr MOD expr           { $$.treeNode = make_op($1.treeNode, Ops::Mod, $3.treeNode); }
            | MINUS expr %prec UMINUS { $$.treeNode = make_op(make_value(0), Ops::Minus, $2.treeNode); }
            | NAME                    { 
                                        $$.treeNode = currentScope->visible($1.name);
                                        if (!$$.treeNode) {
                                          YYLTYPE * info = &@1;
                                          PrintError("Using undeclared variable");
                                        }
                                      }
            | VALUE                   { $$.treeNode = make_value($1.value); }
            | INPUT                   { $$.treeNode = make_op(nullptr, Ops::StdIn, nullptr); }
            | LPAR expr RPAR          { $$.treeNode = $2.treeNode; }

if:           if_head stm             { 
                                        currentScope->addBranch($2.treeNode);
                                        $$.treeNode = make_if($1.treeNode, currentScope);
                                      }
            | if_head scope           { 
                                        currentScope->addBranch($2.treeNode);
                                        $$.treeNode = make_if($1.treeNode, currentScope);
                                      }

if_head:      IF LPAR lexpr RPAR      {
                                        currentScope = currentScope->push();
                                        $$ = $3;
                                      }

lexpr:        expr                    { $$ = $1; }
            | expr RELOP expr         { $$.treeNode = make_op($1.treeNode, $2.op, $3.treeNode); }
            | lexpr OR lexpr          { $$.treeNode = make_op($1.treeNode, Ops::Or, $3.treeNode); }
            | lexpr AND lexpr         { $$.treeNode = make_op($1.treeNode, Ops::And, $3.treeNode); }
            | NOT lexpr               { $$.treeNode = make_op(nullptr, Ops::Not, $2.treeNode); }
            | LPAR lexpr RPAR         { $$ = $2; }

while:        while_head stm          { 
                                        currentScope->addBranch($2.treeNode);
                                        $$.treeNode = make_while($1.treeNode, currentScope);
                                      }
            | while_head scope        { 
                                        currentScope->addBranch($2.treeNode);
                                        $$.treeNode = make_while($1.treeNode, currentScope);
                                      }

while_head:   WHILE LPAR lexpr RPAR   {
                                        currentScope = currentScope->push();
                                        $$ = $3;
                                      }

output:       OUTPUT expr SM          { $$.treeNode = make_op(nullptr, Ops::StdOut, $2.treeNode); }

%%
    
int yyerror(const char *s) {
  PrintError(s);
  return 0;
}

int wholeProgramAction() {
#if (CODEGEN == 1)
  currentScope->codegen();
#else
  currentScope->calc();
#endif
  return 0;
}

