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
    int number;
    node_t * node;
    tree_node * tree;
}

%token PRINT INIT_INT NEW_LINE
%token <str> VARIABLE
%token <number> NUMBER

%type <node> block
%type <node> var_exp
%type <node> num_exp
%type <node> instruction
%type <node> initialization
%type <node> declaration

%type <tree> line_list

%start line_list
%parse-param {tree_node ** code}

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
    ;

print_statement:
    ;
initialization:
    INIT_INT var_exp '=' num_exp   {$$ = (node_t*)createInitializeNode($2, $4);}
    ;
declaration:
    ;
var_exp:
    VARIABLE                            {$$ = (node_t*)createVariableNode($1);}
num_exp:
    NUMBER                              {$$ = (node_t*)createConstantNode($1);}

%%
void yyerror(tree_node ** code, char * msg) {
    fprintf(stderr, msg);
    exit(1);
}

int main() {
    tree_node * cd;
    yyparse(&cd);
    while(cd != NULL) {
        exec(cd->root);
        cd = cd->next;
    }
    printf("TODO OK\n");
}

