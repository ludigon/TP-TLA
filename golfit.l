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
%option yylineno
variable[a-zA-Z][a-zA-Z0-9]*

%%
"#"             {return INIT_INT;}
"^"             {return INIT_STR;}
"_"             {return PRINT;}
\"[^\n]*\"      {yylval.str_lit = malloc(strlen(yytext) + 1);
                                                strcpy(yylval.str_lit, yytext);
                                                return STRING;}
(0|[1-9][0-9]*) {yylval.number = atoi(yytext); return NUMBER;}
"=="            {return IS_EQUAL;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return DIVIDED_BY;}
"<"             {return LESS_THAN;}
"<="            {return LESS_EQUAL_THAN;}
">"             {return GREATER_THAN;}
">="            {return GREATER_EQUAL_THAN;}
"%"             {return REMAINDER;}
"!"             {return NOT;}
"&"             {return AND;}
"?"             {return IF;}
":"             {return ELSE;}
"@"             {return WHILE;}
"|"             {printf("ENCONTRE UN NOT"); return OR;}
"="             {return '=';}
"{"             {return '{';}
"}"             {return '}';}
[\(\)]          {return yytext[0];}
{variable}      {strcpy(yylval.str, yytext); return VARIABLE;}
[\n]+           {return NEW_LINE;}
[ \t]             {;}
.               {yyerror("Syntax Error: Unexpected character\n");}
%%
