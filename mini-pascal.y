
%code requires{
    #include "ast.h"
}
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

%union{
    Expression * expr_t;
    char * string_t;
    char char_t;
    int int_t;
    float float_t;
    bool bool_t;
    Statement * statement_t;
    list<Expression *> * expr_list_t;
    list<Statement *> * statement_list_t;
    list<Declaration *> * declaration_list_t;
    Declaration * declaration_t;
    VarDeclarationStatement * var_declaration_t;
    list<string> * string_list_t;
    ComplexType * type_t;
}

%token TK_LESS_EQUAL TK_GT_EQUAL TK_ASSIGN TK_NOT_EQUAL TK_AND TK_OR TK_NOT KW_PROCEDURE
%token KW_PROGRAM KW_BEGIN KW_END KW_WRITELN KW_IF KW_THEN KW_ELSE KW_WHILE
%token KW_DO KW_FUNCTION KW_READLN KW_VAR KW_CONST KW_INTEGER KW_REAL KW_BOOLEAN
%token KW_CHAR KW_STRING
%token KW_ARRAY KW_OF KW_FOR KW_TO
%precedence KW_THEN
%precedence KW_ELSE

%token<string_t> TK_ID TK_LIT_STRING
%token<char_t> TK_LIT_CHAR
%token<int_t> TK_LIT_INT
%token<float_t> TK_LIT_FLOAT
%token<bool_t> KW_TRUE KW_FALSE
%type<expr_t> factor literal expression arithmetic_expression term unary_expression
%type<statement_t> statement assignation_statement block_statement program
%type<expr_list_t> argument_list
%type<statement_list_t> statement_list
%type<declaration_list_t> declarations optional_const_declarations optional_var_declarations const_declarations var_declarations procedure_function_declarations
%type<declaration_list_t> procedure_declarations function_declarations
%type<declaration_t> procedure_declaration function_declaration const_declaration
%type<var_declaration_t> var_declaration
%type<string_list_t> id_list
%type<int_t> type_identifier
%type<type_t> type
%%

program: KW_PROGRAM TK_ID ';'
            block_statement
           '.' { $$ = new MainStatement($2, $4, line, column); $$->evaluateSemantic();}
       ;

block_statement:
            declarations
            KW_BEGIN
            statement_list
            KW_END { $$ = new BlockStatement($1, $3, line, column); }
        ; 

declarations: optional_const_declarations
              optional_var_declarations
              procedure_function_declarations { $$ = $1;
                list<Declaration *>::iterator it = $2->begin();
                while(it != $2->end()){
                    $$->push_back((*it));
                    it++;
                }
            
                it = $3->begin();
                while(it != $3->end()){
                    $$->push_back((*it));
                    it++;
                }
            }
            ;

optional_const_declarations: KW_CONST const_declarations { $$  = $2;}
                           | /* epsilon */ { $$ = new list<Declaration *>; }
                           ;

optional_var_declarations: KW_VAR var_declarations { $$ = $2;}
                         | /* epsilon */ { $$ = new list<Declaration *>; }
                         ;

procedure_function_declarations: procedure_declarations
                                 function_declarations { $$ = $1;
                                    list<Declaration *>::iterator it = $2->begin();
                                    while(it != $2->end()){
                                        $$->push_back((*it));
                                        it++;
                                    }
                                  }
                               | procedure_declarations { $$ = $1; }
                               | function_declarations { $$ = $1; }
                               | /* epsilon */ {$$ = new list<Declaration *>;}
                               ;

procedure_declarations: procedure_declaration procedure_declarations { $$ = $2; $$->push_back($1);}
                      | procedure_declaration { $$ = new list<Declaration *>; $$->push_back($1);}
                      ;

function_declarations: function_declarations function_declaration { $$ = $1; $$->push_back($2);}
                     | function_declaration { $$ = new list<Declaration *>; $$->push_back($1);}
                     ;

procedure_declaration: KW_PROCEDURE TK_ID '(' var_declaration ')' ';' block_statement ';' { $$ = new ProcedureDeclarationStatement($2, $4, $7, line, column);}
                     ;

function_declaration: KW_FUNCTION TK_ID '(' var_declaration ')' ':' type ';' block_statement ';' { $$ = new FunctionDeclarationStatement($2, $4, $9, $7, line, column);}
                    ;

const_declarations: const_declaration ';' const_declarations {$$ = $3; $$->push_front($1);}
                  | const_declaration ';' {$$ = new list<Declaration *>; $$->push_back($1);}
                  ;

var_declarations: var_declaration ';' var_declarations {$$ = $3; $$->push_front($1);}
                | var_declaration ';' {$$ = new list<Declaration *>; $$->push_back($1);}
                ;

var_declaration: id_list ':' type { $$ = new VarDeclarationStatement($1, $3, line, column); }

id_list: id_list ',' TK_ID { $$ = $1; $$->push_back($3);}
       | TK_ID { $$ = new list<string>; $$->push_back($1);}
       ;

const_declaration: TK_ID ':' type '=' literal { $$ = new ConstDeclarationStatement($1, $3, $5, line, column);}
                  ;

literal: TK_LIT_STRING { $$ = new StringExpr($1, line, column); }
       | TK_LIT_CHAR { $$ = new CharExpr($1, line, column); }
       | TK_LIT_FLOAT { $$ = new FloatExpr($1, line, column); }
       | TK_LIT_INT { $$ = new IntExpr($1, line, column);}
       | KW_TRUE { $$ = new BoolExpr($1, line, column); }
       | KW_FALSE { $$ = new BoolExpr($1, line, column);}
       ;

type: type_identifier { $$ = new ComplexType((PrimitiveType)$1, false); }
    | KW_ARRAY '[' TK_LIT_INT '.''.' TK_LIT_INT ']' KW_OF type_identifier { $$ = new ArrayType($3, $6, (PrimitiveType)$9); }
    ;

type_identifier: KW_CHAR { $$ = CHAR; }
    | KW_STRING { $$ = STRING; }
    | KW_REAL { $$ = REAL; }
    | KW_INTEGER { $$ = INTEGER; }
    | KW_BOOLEAN { $$ = BOOLEAN;}
    ;

statement_list: statement_list statement { $$ = $1; $$->push_back($2); }
              | statement { $$ = new list<Statement *>; $$->push_back($1);}
              ;

statement: KW_WRITELN '(' argument_list ')' ';'  { $$ = new WriteStatement($3, line, column);}
        | KW_READLN '(' argument_list ')' ';' { $$ = new ReadStatement($3, line, column);}
        | KW_IF expression KW_THEN statement  { $$ = new IfStatement($2, $4, NULL, line, column); }
        | KW_IF expression KW_THEN statement KW_ELSE statement  { $$ = new IfStatement($2, $4, $6, line, column); }
        | assignation_statement {$$ = $1;}
        | TK_ID '(' argument_list ')' ';' { $$ = new ExpressionStatement(new MethodInvocationExpr(new IdExpr($1, line, column), $3, line, column), line , column); }
        | KW_WHILE expression KW_DO statement { $$ = new WhileStatement($2, $4, line, column); }
        | KW_FOR TK_ID TK_ASSIGN expression KW_TO expression KW_DO statement { $$ = new ForStatement($2, $4, $6, $8, line, column); }
        | block_statement ';' {$$ = $1;}
        ;

assignation_statement: TK_ID TK_ASSIGN expression ';' { $$ = new AssignationStatement($1, $3, NULL, line, column); }
                     | TK_ID '[' expression ']' TK_ASSIGN expression ';' { $$ = new AssignationStatement($1, $6, $3, line, column);  }
                     ;

argument_list: argument_list ',' expression { $$ = $1; $$->push_back($3);}
             | expression { $$ = new list<Expression *>; $$->push_back($1);}
             ;

expression: expression '>' arithmetic_expression { $$ = new GtExpr($1, $3, line, column);}
          | expression '<' arithmetic_expression { $$ = new LtExpr($1, $3, line, column);}
          | expression TK_LESS_EQUAL arithmetic_expression { $$ = new LteExpr($1, $3, line, column);}
          | expression TK_GT_EQUAL arithmetic_expression { $$ = new GteExpr($1, $3, line, column);}
          | expression TK_NOT_EQUAL arithmetic_expression { $$ = new NeqExpr($1, $3, line, column);}
          | expression '=' arithmetic_expression { $$ = new EqExpr($1, $3, line, column);}
          | arithmetic_expression {$$ = $1;}
          ;

arithmetic_expression: arithmetic_expression '+' term { $$ = new AddExpr($1, $3, line, column);}
                     | arithmetic_expression '-' term { $$ = new SubExpr($1, $3, line, column);}
                     | arithmetic_expression TK_OR term { $$ = new OrExpr($1, $3, line, column);}
                     | term {$$ = $1;}
                     ;

term: term '*' unary_expression { $$ = new MultExpr($1, $3, line, column);}
    | term '/' unary_expression { $$ = new DivExpr($1, $3, line, column);}
    | term '%' unary_expression { $$ = new ModExpr($1, $3, line, column);}
    | term TK_AND unary_expression { $$ = new AndExpr($1, $3, line, column);}
    | unary_expression { $$ = $1; }
    ;

unary_expression: TK_NOT factor { $$ = new UnaryExpr(NOT, $2, line, column); }
                | factor { $$ = $1; }
                ;

factor: '(' expression ')' {$$ = $2;}
      | literal { $$ = $1;}
      | TK_ID { $$ = new IdExpr($1, line, column); }
      | TK_ID '[' factor']' { $$ = new ArrayExpr(new IdExpr($1, line, column), $3, line, column); }
      | TK_ID '(' argument_list ')' ';' { $$ = new MethodInvocationExpr(new IdExpr($1, line, column), $3, line, column); }
      ;
%%