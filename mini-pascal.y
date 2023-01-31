%{
    #include <cstdio>
    extern int yylineno;
    int yylex();
    extern int line;
    extern int column;
    void yyerror(const char * message){
        fprintf(stderr, "Error: %s in line: %d and column %d", message, line, column);
    }
%}

%token TK_LESS_EQUAL TK_GT_EQUAL TK_ASSIGN TK_NOT_EQUAL TK_AND TK_OR TK_NOT
%token KW_PROGRAM KW_BEGIN KW_END KW_WRITELN KW_IF KW_THEN KW_ELSE KW_WHILE
%token KW_DO KW_FUNCTION KW_READLN KW_VAR KW_CONST KW_INTEGER KW_REAL KW_BOOLEAN
%token KW_CHAR KW_STRING TK_LIT_STRING TK_LIT_CHAR TK_LIT_INT TK_LIT_FLOAT TK_ID

%%

program: KW_PROGRAM TK_ID ';'
            declarations
            KW_BEGIN
            statement_list
            KW_END '.'
       ;

declarations: KW_CONST const_declarations KW_VAR var_declarations
            ;

const_declarations: const_declarations ';' const_declaration
                  | const_declaration ';'
                  | /* epsilon */
                  ;

var_declarations: var_declarations ';' var_declaration
                | var_declaration ';'
                | /* epsilon */
                ;

var_declaration: id_list ':' type

id_list: id_list ',' TK_ID
       | TK_ID
       ;

const_declaration: TK_ID '=' literal
                  | TK_ID ':' type '=' literal
                  ;

literal: TK_LIT_STRING
       | TK_LIT_CHAR
       | TK_LIT_FLOAT
       | TK_LIT_INT
       ;

type: KW_CHAR
    | KW_STRING
    | KW_REAL
    | KW_INTEGER
    ;

statement_list: statement_list statement
              | statement
              ;

statement: KW_WRITELN '(' argument_list ')' ';'
        ;

argument_list: argument_list ',' expression
             | expression
             ;

expression: expression '>' arithmetic_expression
          | expression '<' arithmetic_expression
          | expression TK_LESS_EQUAL arithmetic_expression
          | expression TK_GT_EQUAL arithmetic_expression
          | expression TK_NOT_EQUAL arithmetic_expression
          | expression '=' arithmetic_expression
          | arithmetic_expression
          ;

arithmetic_expression: arithmetic_expression '+' term
                     | arithmetic_expression '-' term
                     | term
                     ;

term: term '*' factor
    | term '/' factor
    | term '%' factor
    | factor
    ;

factor: '(' expression ')'
      | literal
      | TK_ID
      ;
%%