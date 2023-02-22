%option noyywrap
%{
    #include "tokens.h"
    int line = 1;
    int column = 0;
%}

LETTER [a-zA-Z]
DIGIT [0-9]

%x commenttype1
%x commenttype2

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
"{" { BEGIN(commenttype1); } 
<commenttype1>[\n] { line++; }
<commenttype1>"}" { BEGIN(INITIAL); }
<commenttype1>. {/* NADA */}
"(*" { BEGIN(commenttype2); }
<commenttype2>[\n] { line++; }
<commenttype2>"*)" { BEGIN(INITIAL); }
<commenttype2>. {/* NADA */}
[ \t] { column++; }
[ \n \r] { line++; }
"program" {return KW_PROGRAM;}
"procedure" {return KW_PROCEDURE;}
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
"true" { yylval.bool_t = true; return KW_TRUE; }
"false" { yylval.bool_t = false; return KW_FALSE; }
"array" { return KW_ARRAY; }
"of" { return KW_OF; }
"for" { return KW_FOR; }
"to" { return KW_TO; }
"[" { return '['; }
"]" { return ']'; }
"'"[^']{0,1}"'" { yylval.char_t = yytext[0]; return TK_LIT_CHAR;}
"'"[^']*"'" { yylval.string_t = strdup(yytext); return TK_LIT_STRING;}
{DIGIT}+ {return TK_LIT_INT;}
{DIGIT}+"."{DIGIT}+ { yylval.float_t = atof(yytext); return TK_LIT_FLOAT;}
{LETTER}({DIGIT}|{LETTER}|_)* { yylval.string_t = strdup(yytext); return TK_ID;}

. {printf("caracter invalido %c en la linea: %d\n", yytext[0], yylineno);}
%%