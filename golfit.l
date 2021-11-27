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
\"[a-zA-Z0-9 \n\t\_\#\=\+\-\*\/\%]*\"        {  yylval.str_lit = malloc(strlen(yytext) + 1);
                                                strcpy(yylval.str_lit, yytext);
                                                return STRING;}
"#"             {return INIT_INT;}
"^"             {return INIT_STR;}
"_"             {return PRINT;}
\-?[1-9][0-9]*  {yylval.number = atoi(yytext); return NUMBER;}
"="             {return '=';}
"+"             {yylval.ch = *yytext; return PLUS;}
"-"             {yylval.ch = *yytext; return MINUS;}
"*"             {yylval.ch = *yytext; return TIMES;}
"/"             {yylval.ch = *yytext; return DIVIDED_BY;}
"%"             {yylval.ch = *yytext; return REMAINDER;}
[\(\)]          {return yytext[0];}
{variable}      {strcpy(yylval.str, yytext); return VARIABLE;}
"\n"            {return NEW_LINE;}
[ ]             {;}
.
%%