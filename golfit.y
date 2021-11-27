%{
#include <stdio.h>
#include <stdlib.h>
#include "nodes.h"
#include "nodeGen.h"
#include "treeReductor.h"
extern int yylineno;
int yylex();
void yyerror(tree_node **, char *);    
%}

%union {
    char str[256];
    char * str_lit;
    int number;
    char ch;
    node_t * node;
    tree_node * tree;
}

%token PRINT INIT_INT NEW_LINE INIT_STR
%token <str> VARIABLE
%token <str_lit> STRING
%token <number> NUMBER
%token <ch> PLUS MINUS TIMES DIVIDED_BY REMAINDER

%type <node> block
%type <node> expr
%type <node> var_exp
%type <node> num_exp
%type <node> str_exp
%type <node> instruction
%type <node> initialization
%type <node> declaration
%type <node> print_statement

%type <tree> line_list

%left PLUS MINUS
%left TIMES DIVIDED_BY REMAINDER

%start line_list
%parse-param {tree_node ** code}
%define parse.error verbose

%%
line_list:
    block                               {$$ = (*(code) = generateTree($1));}
    | line_list NEW_LINE block          {$$ = (*(code) = addTree($1, $3));}
    ;
block:
    instruction                         {$$ = $1;}
    ;

instruction:
    initialization                          {$$ = $1;}
    | declaration                           {$$ = $1;}
    | print_statement                       {$$ = $1;}
    ;

print_statement:
    PRINT expr                              {$$ = (node_t*)createPrintNode($2);}
    ;

expr:
    var_exp                                 {$$ = $1;}
    | num_exp                               {$$ = $1;}
    | str_exp                               {$$ = $1;}

initialization:
    INIT_INT var_exp '=' num_exp     {$$ = (node_t*)createInitializeNode($2, $4, INT_VAR);}
    | INIT_INT var_exp '=' var_exp   {$$ = (node_t*)createInitializeNode($2, $4, INT_VAR);}
    | INIT_STR var_exp '=' str_exp   {$$ = (node_t*)createInitializeNode($2, $4, STR_VAR);}
    | INIT_STR var_exp '=' var_exp   {$$ = (node_t*)createInitializeNode($2, $4, STR_VAR);}
    ;
declaration:
    INIT_INT var_exp               {$$ = (node_t*)createInitializeNode($2, (node_t*)createConstantNode(0), INT_VAR);}
    ;
var_exp:
    VARIABLE                            {$$ = (node_t*)createVariableNode($1);}
    ;
str_exp:
    STRING                            {$$ = (node_t*)createStringNode($1);}
    ;

num_exp:
    NUMBER                              {$$ = (node_t*)createConstantNode($1);}
    | num_exp PLUS num_exp              {$$ = (node_t*)createOperationNode($1, $2, $3);}
    | num_exp MINUS num_exp             {$$ = (node_t*)createOperationNode($1, $2, $3);}
    | num_exp TIMES num_exp             {$$ = (node_t*)createOperationNode($1, $2, $3);}
    | num_exp DIVIDED_BY num_exp        {$$ = (node_t*)createOperationNode($1, $2, $3);}
    | num_exp REMAINDER num_exp         {$$ = (node_t*)createOperationNode($1, $2, $3);}
    | '(' num_exp ')'                   {$$ = $2;}
    ;


%%
void yyerror(tree_node ** code, char * msg) {
    fprintf(stderr, msg);
    exit(1);
}

int main() {
    tree_node * cd;
    yyparse(&cd);
    while(cd != NULL) {
        printf("%s\n", exec(cd->root));
        cd = cd->next;
    }
    printf("TODO OK\n");
}

