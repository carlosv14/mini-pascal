#include <string>
#include <list>
#include <vector>
#include "CodeContext.h"

using namespace std;

class Node{
    public:
        int line;
        int column;
        Node(int line, int column){
            this->line = line;
            this->column = column;
        }
        virtual void print() = 0;
};

class Statement : public Node{
    public:
        Statement(int line, int column) : Node(line, column){}
        virtual void print() = 0;
        virtual void evaluateSemantic() = 0;
        virtual string generateCode() = 0;
};

class DeclarationStatement : public Statement{
    public:
        DeclarationStatement(int line, int column) : Statement(line, column){}
        virtual void print() = 0;
        virtual void evaluateSemantic() = 0;
        virtual string generateCode() = 0;
};

class Expression : public Node{
    public:
        Expression(int line, int column) : Node(line, column){

        }
        virtual void print() = 0;
        virtual ComplexType* getType() = 0;
        virtual void generateCode(CodeContext &context) = 0;
};

class IdExpr : public Expression{
    public:
        IdExpr(string id, int line, int column) : Expression(line, column){
            this->id = id;
            this->line = line;
            this->column = column;
        }
        string id;
        void print();
        ComplexType* getType();
        void generateCode(CodeContext &context);
};

class ArrayExpr : public Expression{
    public:
        ArrayExpr(
            IdExpr * idExpr,
            Expression * index,
            int line,
            int column) : Expression(line, column){
            this->id = idExpr;
            this->index = index;
        }
        IdExpr * id;
        Expression * index;
        void print();
        ComplexType* getType();
        void generateCode(CodeContext &context);
};

class StringExpr : public Expression{
    public:
        StringExpr(string value, int line, int column) : Expression(line, column){
            this->value = value;
        }
        string value;
        void print();
        ComplexType* getType();
        void generateCode(CodeContext &context);
};

class CharExpr : public Expression{
    public:
        CharExpr(char value, int line, int column) : Expression(line, column){
            this->value = value;
        }
        char value;
        void print();
        ComplexType* getType();
        void generateCode(CodeContext &context);
};

class FloatExpr : public Expression{
    public:
        FloatExpr(float value, int line, int column) : Expression(line, column){
            this->value = value;
        }
        float value;
        void print();
        ComplexType* getType();
        void generateCode(CodeContext &context);
};

class IntExpr : public Expression{
    public:
        IntExpr(int value, int line, int column) : Expression(line, column){
            this->value = value;
        }
        int value;
        void print();
        ComplexType* getType();
        void generateCode(CodeContext &context);
};

class BoolExpr : public Expression{
    public:
        BoolExpr(bool value, int line, int column) : Expression(line, column){
            this->value = value;
        }
        bool value;
        void print();
        ComplexType* getType();
        void generateCode(CodeContext &context);
};

class UnaryExpr : public Expression{
    public:
        UnaryExpr(UnaryOperator op, Expression * expr, int line, int column) : 
            Expression(line, column){
            this->op = op;
            this->expr = expr;
        }
        UnaryOperator op;
        Expression * expr;
        void print();
        ComplexType* getType();
        void generateCode(CodeContext &context);
};

class BinaryExpr : public Expression{
    public:
        BinaryExpr(Expression * left, Expression * right, int line, int column) : 
            Expression(line, column){
            this->left = left;
            this->right = right;
        }
        Expression * left;
        Expression * right;
        virtual void print() = 0;
        virtual ComplexType* getType() = 0;
        virtual void generateCode(CodeContext &context) = 0;
};

#define IMPLEMENT_BINARY_EXPR(name)\
class name##Expr : public BinaryExpr{\
    public: \
        name##Expr(Expression * left, Expression * right, int line, int column): BinaryExpr(left, right, line, column){}\
        void print();\
        ComplexType* getType();\
        void generateCode(CodeContext &context);\
};


class WriteStatement : public Statement{
    public:
        WriteStatement(list<Expression *> * expressions, int line, int column) :
            Statement(line, column){
            this->expressions = expressions;
        }
        list<Expression *> * expressions;
        void print();
        void evaluateSemantic();
        string generateCode();
};

class ReadStatement : public Statement{
    public:
        ReadStatement(list<Expression *> * expressions, int line, int column) :
            Statement(line, column){
            this->expressions = expressions;
        }
        list<Expression *> * expressions;
        void print();
        void evaluateSemantic();
        string generateCode();
};

class IfStatement : public Statement{
    public:
        IfStatement(Expression * expression, Statement * trueStatement, Statement * falseStatement, int line, int column) :
            Statement(line, column){
            this->expression = expression;
            this->trueStatement = trueStatement;
            this->falseStatement = falseStatement;
        }
        Expression * expression;
        Statement * trueStatement;
        Statement * falseStatement;
        void print();
        void evaluateSemantic();
        string generateCode();
};

class MethodInvocationExpr : public Expression{
    public:
        MethodInvocationExpr(IdExpr * id, list<Expression *> * args, int line, int column)
            : Expression(line, column)
        {
            this-> id = id;
            this->args = args;
        }
        IdExpr * id;
        list<Expression *> * args;
        void print();
        ComplexType* getType();
        void generateCode(CodeContext &context);
        string generatCode();
};

class ExpressionStatement : public Statement{
    public:
        ExpressionStatement(Expression * expr, int line, int column)
            : Statement(line, column){
            this->expr = expr;
        }
        Expression * expr;
        void print();
        void evaluateSemantic();
        string generateCode();
};

class AssignationStatement : public Statement{
    public:
        AssignationStatement(string id, Expression * expr, Expression * index, int line, int column)
            : Statement(line, column){
            this->id = id;
            this->expr = expr;
            this->index = index;
            this->isArray = index != NULL;
        }
        string id;
        Expression * expr;
        Expression * index;
        bool isArray;
        void print();
        void evaluateSemantic();
        string generateCode();
};

class WhileStatement : public Statement{
    public:
        WhileStatement(Expression * expression, Statement * stmt, int line, int column)
            : Statement(line, column){
            this->expression = expression;
            this->stmt = stmt;
        }
        Expression * expression;
        Statement * stmt;
        void print();
        void evaluateSemantic();
        string generateCode();
};

class ForStatement: public Statement{
    public:
        ForStatement(string id, Expression * fromExpr, Expression * toExpr, Statement * stmt, int line, int column)
            : Statement(line, column){
            this->id = id;
            this->fromExpr = fromExpr;
            this->toExpr = toExpr;
            this->stmt = stmt;
        }
        string id;
        Expression * fromExpr;
        Expression * toExpr;
        Statement * stmt;
        void print();
        void evaluateSemantic();
        string generateCode();
};

class MainStatement: public Statement{
    public:
        MainStatement(string id, list<DeclarationStatement *> * declarations, list<Statement * > * stmts, int line, int column) : 
            Statement(line, column){
            this->id = id;
            this->stmts = stmts;
            this->declarations = declarations;
        }
        string id;
        list<DeclarationStatement *> * declarations;
        list<Statement * > * stmts;
        void print();
        void evaluateSemantic();
        string generateCode();
};


class VarDeclarationStatement : public DeclarationStatement{
    public:
        VarDeclarationStatement(list<string> * ids, ComplexType * type, int line, int column)
            : DeclarationStatement(line, column){
            this->ids = ids;
            this->type = type;
        }
        list<string> * ids;
        ComplexType * type;
        void print();
        void evaluateSemantic();
        string generateCode();
};


class ConstDeclarationStatement : public DeclarationStatement{
    public:
        ConstDeclarationStatement(string id, ComplexType * type, Expression * literal, int line, int column)
            : DeclarationStatement(line, column){
            this->id = id;
            this->type = type;
            this->literal = literal;
        }
        string id;
        ComplexType * type;
        Expression * literal;
        void print();
        void evaluateSemantic();
        string generateCode();
};
class ProcedureDeclarationStatement: public DeclarationStatement{
    public:
        ProcedureDeclarationStatement(string id, VarDeclarationStatement * varDeclaration, Statement * statement, int line, int column)
            : DeclarationStatement(line, column){
            this->id = id;
            this->varDeclaration = varDeclaration;
            this->statement = statement;
        }
        string id;
        VarDeclarationStatement * varDeclaration;
        Statement * statement;
        void print();
        void evaluateSemantic();
        string generateCode();
};

class FunctionDeclarationStatement : public DeclarationStatement{
    public:
        FunctionDeclarationStatement(string id, VarDeclarationStatement * varDeclaration, Statement * statement, ComplexType * type, int line, int column)
            : DeclarationStatement(line, column){
            this->id = id;
            this->varDeclaration = varDeclaration;
            this->statement = statement;
            this->type = type;

        }
        string id;
        VarDeclarationStatement * varDeclaration;
        Statement * statement;
        ComplexType * type;
        void print();
        void evaluateSemantic();
        string generateCode();
};

class BlockStatement: public Statement{
    public:
        BlockStatement(list<DeclarationStatement *> * declarations, list<Statement * > * stmts, int line, int column)
            : Statement(line, column){
            this->declarations = declarations;
            this->stmts = stmts;
        }
        list<DeclarationStatement *> * declarations;
        list<Statement * > * stmts;
        void print();
        void evaluateSemantic();
        string generateCode();
};

class MethodInformation{
    public:
        ComplexType * returnType;
        VarDeclarationStatement * parameters;
        MethodInformation(ComplexType * returnType, VarDeclarationStatement * parameters){
            this->returnType = returnType;
            this->parameters = parameters;
        }
};

IMPLEMENT_BINARY_EXPR(And);
IMPLEMENT_BINARY_EXPR(Mod);
IMPLEMENT_BINARY_EXPR(Div);
IMPLEMENT_BINARY_EXPR(Mult);
IMPLEMENT_BINARY_EXPR(Or);
IMPLEMENT_BINARY_EXPR(Sub);
IMPLEMENT_BINARY_EXPR(Add);
IMPLEMENT_BINARY_EXPR(Eq);
IMPLEMENT_BINARY_EXPR(Neq);
IMPLEMENT_BINARY_EXPR(Gt);
IMPLEMENT_BINARY_EXPR(Lt);
IMPLEMENT_BINARY_EXPR(Gte);
IMPLEMENT_BINARY_EXPR(Lte);
