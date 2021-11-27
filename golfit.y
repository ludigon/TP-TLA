%{
#include <stdio.h>
#include <stdlib.h>
extern int yylineno;
int yylex();
void yyerror(char *);    
%}

%union {
    char str[256];
    int number;
}

%token PRINT ASSIGN_INT NUMBER VARIABLE
%type <str> VARIABLE;
%type <number> NUMBER;
%start line_list

%%
line_list:
    block
    | line_list block

block:
    instruction
    ;

instruction:
    assignment
    | declaration
    | print_statement
    ;

print_statement:
    PRINT NUMBER
    | PRINT VARIABLE
    ;
assignment:
    ASSIGN_INT VARIABLE '=' NUMBER   {printf("int %s = %d;\n", $2, $4);}
    ;
declaration:
    ASSIGN_INT VARIABLE {printf("int %s", $2);}

%%

void yyerror(char * msg) {
    fprintf(stderr, msg);
    exit(1);
}

int main() {
    yyparse();
    printf("TODO OK\n");
}

