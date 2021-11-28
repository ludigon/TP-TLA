%{
#include <stdio.h>
#include <stdlib.h>
#include "nodes.h"
#include "nodeGen.h"
#include "treeReductor.h"
extern int yylineno;
int yylex();
void yyerror(tree_node **,const char *);    
%}

%union {
    char str[256];
    char * str_lit;
    int number;
    char ch;
    node_t * node;
    tree_node * tree;
}

%token INPUT PRINT INIT_INT NEW_LINE INIT_STR
%token <str> VARIABLE
%token <str_lit> STRING
%token <number> NUMBER
%token <str> PLUS MINUS TIMES DIVIDED_BY REMAINDER LESS_THAN LESS_EQUAL_THAN GREATER_THAN GREATER_EQUAL_THAN AND OR NOT IF ELSE WHILE


%type <node> block
%type <node> expr
%type <node> var_exp
%type <node> num_exp
%type <node> str_exp
%type <node> single_var
%type <node> instruction
%type <node> initialization
%type <node> assignation
%type <node> declaration
%type <node> print_statement
%type <node> input_statement
%type <node> while
%type <node> if

%type <tree> line_list
//El orden en el que los declaramos indica la precedencia, siendo el OR el que menos precedencia tiene y el NOT el que mas
%left OR
%left AND
%left IS_EQUAL
%left LESS_EQUAL_THAN LESS_THAN GREATER_EQUAL_THAN GREATER_THAN
%left PLUS MINUS
%left TIMES DIVIDED_BY REMAINDER
%right NOT

%nonassoc THEN
%nonassoc ELSE

%start line_list
%parse-param {tree_node ** code}

%%
line_list:
    block                                   {$$ = (*(code) = generateTree($1));}
    | line_list NEW_LINE block              {$$ = (*(code) = addTree($1, $3));}
    | line_list NEW_LINE                    {$$ = (*(code) = addTree($1, NULL));}
    ;
block:
    instruction                             {$$ = $1;}
	| while									{$$ = $1;}
	| if									{$$ = $1;}
	| '{' NEW_LINE line_list NEW_LINE '}' 	{$$ = (node_t*)createLineListNode($3);}
    ;

instruction:
    initialization                          {$$ = $1;}
    | declaration                           {$$ = $1;}
    | print_statement                       {$$ = $1;}
    | input_statement                       {$$ = $1;}
    | assignation                           {$$ = $1;}
    ;

print_statement:
    PRINT expr                              {$$ = (node_t*)createPrintNode($2);}
    ;

input_statement:
    INPUT single_var                        {$$ = (node_t*)createInputNode($2);}
    ;

expr:
    var_exp                                 {$$ = $1;}
    | str_exp                               {$$ = $1;}

initialization:
    INIT_INT single_var '=' var_exp         {$$ = (node_t*)createInitializeNode($2, $4, INT_VAR);}
    | INIT_STR single_var '=' str_exp       {$$ = (node_t*)createInitializeNode($2, $4, STR_VAR);}
    ;
declaration:
    INIT_INT single_var                     {$$ = (node_t*)createInitializeNode($2, (node_t*)createConstantNode(0), INT_VAR);}
    ;
assignation:
    single_var '=' var_exp                  {$$ = (node_t*)createAssignNode($1, $3);}
    | single_var '=' str_exp                {$$ = (node_t*)createAssignNode($1, $3);}
    ;

single_var:
    VARIABLE                                {$$ = (node_t*)createVariableNode($1);}
var_exp:
    single_var                              {$$ = $1;}
    | num_exp                               {$$ = $1;}
    | var_exp PLUS var_exp                  {$$ = (node_t*)createOperationNode($1, "+", $3);}
    | var_exp MINUS var_exp                 {$$ = (node_t*)createOperationNode($1, "-", $3);}
    | var_exp TIMES var_exp                 {$$ = (node_t*)createOperationNode($1, "*", $3);}
    | var_exp DIVIDED_BY var_exp            {$$ = (node_t*)createOperationNode($1, "/", $3);}
    | var_exp REMAINDER var_exp             {$$ = (node_t*)createOperationNode($1, "%", $3);}
    | var_exp LESS_THAN var_exp             {$$ = (node_t*)createOperationNode($1, "<", $3);}
    | var_exp LESS_EQUAL_THAN var_exp       {$$ = (node_t*)createOperationNode($1, "<=", $3);}
    | var_exp GREATER_THAN var_exp          {$$ = (node_t*)createOperationNode($1, ">", $3);}
    | var_exp GREATER_EQUAL_THAN var_exp    {$$ = (node_t*)createOperationNode($1, ">=", $3);}
    | var_exp AND var_exp                   {$$ = (node_t*)createOperationNode($1, "&&", $3);}
    | var_exp OR var_exp                    {$$ = (node_t*)createOperationNode($1, "||", $3);}
    | var_exp IS_EQUAL var_exp              {$$ = (node_t*)createOperationNode($1, "==", $3);}
    | NOT var_exp                           {$$ = (node_t*)createUnaryOperationNode($2, "!");}
    | '(' var_exp ')'                       {$$ = $2;}
    ;
str_exp:
    STRING                                  {$$ = (node_t*)createStringNode($1);}
    ;

num_exp:
    NUMBER                                  {$$ = (node_t*)createConstantNode($1);}
    | MINUS NUMBER                          {$$ = (node_t*)createConstantNode($2 * -1);}
    ;                               

while:
	WHILE expr WHILE block 				    {$$ = (node_t*)createWhileNode($2, $4);}
	;	

if:
	expr IF block 		%prec THEN 			{$$ = (node_t*)createIfNode($1, $3, NULL);}
	| expr IF block ELSE block 			    {$$ = (node_t*)createIfNode($1, $3, $5);}
	;	

%%
void yyerror(tree_node ** code,const char * msg) {
    fprintf(stderr, "Line %d: %s", yylineno, msg);
    exit(1);
}

int main() {
    tree_node * cd;
    yyparse(&cd);
    printf("#include <stdlib.h>\n#include <stdio.h>\n");
    printf("int main() {\n");
    while(cd != NULL) {
		char * node = exec(cd->root);
		if (node != NULL) {
			printf("%s\n", node);
			free(node);
		}
        cd = cd->next;
    }
    printf("}");
    //printf("TODO OK\n");
}

