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

%token TK_LESS_EQUAL TK_GT_EQUAL TK_ASSIGN TK_NOT_EQUAL TK_AND TK_OR TK_NOT KW_PROCEDURE
%token KW_PROGRAM KW_BEGIN KW_END KW_WRITELN KW_IF KW_THEN KW_ELSE KW_WHILE
%token KW_DO KW_FUNCTION KW_READLN KW_VAR KW_CONST KW_INTEGER KW_REAL KW_BOOLEAN
%token KW_CHAR KW_STRING TK_LIT_STRING TK_LIT_CHAR TK_LIT_INT TK_LIT_FLOAT TK_ID
%token KW_TRUE KW_FALSE KW_ARRAY KW_OF
%precedence KW_THEN
%precedence KW_ELSE

%%

program: KW_PROGRAM TK_ID ';'
            block_statement
           '.'
       ;

block_statement:
            declarations
            KW_BEGIN
            statement_list
            KW_END
        ;

declarations: optional_const_declarations
              optional_var_declarations
              procedure_function_declarations
            ;

optional_const_declarations: KW_CONST const_declarations
                           | /* epsilon */
                           ;

optional_var_declarations: KW_VAR var_declarations
                         | /* epsilon */
                         ;

procedure_function_declarations: procedure_declarations
                                 function_declarations
                               | procedure_declarations
                               | function_declarations
                               | /* epsilon */
                               ;

procedure_declarations: procedure_declaration procedure_declarations
                      | procedure_declaration
                      ;

function_declarations: function_declarations function_declaration
                     | function_declaration
                     ;

procedure_declaration: KW_PROCEDURE TK_ID '(' var_declaration ')' ';' block_statement ';'
                     ;

function_declaration: KW_FUNCTION TK_ID '(' var_declaration ')' ':' type ';' block_statement ';'
                    ;

const_declarations: const_declaration ';' const_declarations
                  | const_declaration ';'
                  ;

var_declarations: var_declaration ';' var_declarations
                | var_declaration ';'
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
       | KW_TRUE
       | KW_FALSE
       ;

type: type_identifier
    | KW_ARRAY '[' TK_LIT_INT '.''.' TK_LIT_INT ']' KW_OF type_identifier
    ;

type_identifier: KW_CHAR
    | KW_STRING
    | KW_REAL
    | KW_INTEGER
    | KW_BOOLEAN
    ;

statement_list: statement_list statement
              | statement
              ;

statement: KW_WRITELN '(' argument_list ')' ';'
        | KW_READLN '(' argument_list ')' ';'
        | KW_IF expression KW_THEN statement
        | KW_IF expression KW_THEN statement KW_ELSE statement
        | assignation_statement
        | TK_ID '(' argument_list ')' ';'
        | KW_WHILE expression KW_DO statement
        | block_statement ';'
        ;

assignation_statement: TK_ID TK_ASSIGN expression ';'
                     | TK_ID '[' expression ']' TK_ASSIGN expression ';'
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
                     | arithmetic_expression TK_OR term
                     | term
                     ;

term: term '*' unary_expression
    | term '/' unary_expression
    | term '%' unary_expression
    | term TK_AND unary_expression
    | unary_expression
    ;

unary_expression: TK_NOT factor
                | factor
                ;

factor: '(' expression ')'
      | literal
      | TK_ID
      | TK_ID '[' factor']'
      ;
%%