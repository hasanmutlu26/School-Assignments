%{
#include "y.tab.h"
#define COMMENT 1
#define VALUE 2
#define IDENTIFIER 3
%}

%%
"true"|"false"        {return BinaryValue;}
"and"       {return yytext;}
"or"        {return yytext;}
"not"       {return yytext;}
"equal"     {return yytext;}
"less"      {return yytext;}
"nil"       {return yytext;}
"list"      {return yytext;}
"append"    {return yytext;}
"concat"    {return yytext;}
"set"       {return yytext;}
"deffun"    {return yytext;}
"for"       {return yytext;}
"if"        {return yytext;}
"exit"      {return yytext;}
"load"      {return yytext;}
"disp"      {return yytext;}
"+"         {return yytext[0];}
"-"         {return yytext[0];}
"/"         {return yytext[0];}
"*"         {return yytext[0];}
"**"        {return yytext;}
"\""        {return yytext[0];}
"\'"        {return yytext[0];}
","         {return yytext[0];}
")"         {return yytext[0];}
"("         {return yytext[0];}
" "         {;}
[";;"][a-zA-z0-9]*      {return COMMENT;}
[1-9][0-9]*             {yylval.num = atoi(yytext); return IntegerValue;}
[0]                     {yylval.num = atoi(yytext); return IntegerValue;}
[0-9][.][0-9]*          {yylval.num = atoi(yytext); return VALUE;}
[1-9][0-9]*[.][0-9]*    {yylval.num = atoi(yytext); return VALUE;}
[a-zA-z][a-zA-z0-9]*    {return Id;}
.           {yyerror("SYNTAX_ERROR Expression not recognized\n");}

%%      

int yywrap(void){return 1;}


