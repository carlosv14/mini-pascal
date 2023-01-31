%option noyywrap
%{
    #include "tokens.h"
    int line = 1;
    int column = 0;
%}

LETTER [a-zA-Z]
DIGIT [0-9]

%%

"+" {return '+';}
"-" {return '-';}
"*" {return '*';}
"/" {return '/';}
"(" {return '(';}
")" {return ')';}
";" {return ';';}
"<" {return '<';}
">" {return '>';}
"<=" {return TK_LESS_EQUAL;}
">=" {return TK_GT_EQUAL;}
"=" {return '=';}
":=" {return TK_ASSIGN;}
":" {return ':';}
"%" {return '%';}
"." {return '.';}
"," {return ',';}
"<>" {return TK_NOT_EQUAL;}
"and" {return TK_AND;}
"or" {return TK_OR;}
"not" {return TK_NOT;}
"{" {return '{';}
"}" {return '}';}
[ \t] { column++; }
[ \n \r] { line++; }
"program" {return KW_PROGRAM;}
"begin" {return KW_BEGIN;}
"end" {return KW_END;}
"writeln" {return KW_WRITELN;}
"if" {return KW_IF;}
"then" {return KW_THEN;}
"else" {return KW_ELSE;}
"while" {return KW_WHILE;}
"do" {return KW_DO;}
"function" {return KW_FUNCTION;}
"readln" {return KW_READLN;}
"var" {return KW_VAR;}
"const" {return KW_CONST;}
"integer" {return KW_INTEGER;}
"real" {return KW_REAL;}
"boolean" {return KW_BOOLEAN;}
"char" {return KW_CHAR;}
"string" {return KW_STRING;}
"'"[^']{0,1}"'" {return TK_LIT_CHAR;}
"'"[^']*"'" {return TK_LIT_STRING;}
{DIGIT}+ {return TK_LIT_INT;}
{DIGIT}+"."{DIGIT}+ {return TK_LIT_FLOAT;}
{LETTER}({DIGIT}|{LETTER}|_)* {return TK_ID;}

. {printf("caracter invalido %c en la linea: %d\n", yytext[0], yylineno);}
%%