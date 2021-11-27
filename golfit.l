%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nodeGen.h"
#include "nodes.h"
#include "y.tab.h"
void yyerror(char *);
%}

%option noyywrap

variable[a-zA-Z][a-zA-Z0-9]*

%%
"#"             {return INIT_INT;}
"_"             {return PRINT;}
"="             {return '=';}
"+"             {yylval.ch = *yytext; return PLUS;}
"-"             {yylval.ch = *yytext; return MINUS;}
"*"             {yylval.ch = *yytext; return TIMES;}
"/"             {yylval.ch = *yytext; return DIVIDED_BY;}
[\(\)]          {return yytext[0];}
{variable}      {strcpy(yylval.str, yytext); return VARIABLE;}
[1-9][0-9]*     {yylval.number = atoi(yytext); return NUMBER;}
"\n"            {return NEW_LINE;}
[ ]             {;}
.
%%