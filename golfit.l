%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
void yyerror(char *);
%}

%option noyywrap

variable[a-zA-Z][a-zA-Z0-9]*

%%
"#"             {return ASSIGN_INT;}
"_"             {return PRINT;}
"="             {return '=';}
{variable}      {strcpy(yylval.str, yytext); return VARIABLE;}
[1-9][0-9]*     {yylval.number = atoi(yytext); return NUMBER;}
"\n"            {;}
[ ]             {;}
.
%%