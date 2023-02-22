#include "ast.h"
#include <iostream>

using namespace std;

void IdExpr::print(){
    cout<<"Id expr: "<< this->id <<" line: "<< this->line << " column : "<<this->column<<endl;
}

void ArrayExpr::print(){
    cout<<"Id expr: "<< this->id <<" line: "<< this->line << " column : "<<this->column<<endl;
}

void StringExpr::print(){
    cout<<"string expr: "<< this->value <<" line: "<< this->line << " column : "<<this->column<<endl;
}

void CharExpr::print(){
    cout<<"char expr: "<< this->value <<" line: "<< this->line << " column : "<<this->column<<endl;
}

void FloatExpr::print(){
    cout<<"float expr: "<< this->value <<" line: "<< this->line << " column : "<<this->column<<endl;
}

void IntExpr::print(){
    cout<<"int expr: "<< this->value <<" line: "<< this->line << " column : "<<this->column<<endl;
}

void BoolExpr::print(){
    cout<<"bool expr: "<< this->value <<" line: "<< this->line << " column : "<<this->column<<endl;
}

void UnaryExpr::print(){
    cout<<"unary expr line: "<< this->line << " column : "<<this->column<<endl;
}

#define PRINT_BINARY_EXPR(name)\
void name##Expr::print(){\
    cout<<""<<endl;\
};

void WriteStatement::print(){
    cout<<"Write statement "<<this->line << " column : "<<this->column<<endl;
}

void ReadStatement::print(){
    cout<<"Read statement "<<this->line << " column : "<<this->column<<endl;
}

void IfStatement::print(){
    cout<<"If statement "<<this->line << " column : "<<this->column<<endl;
}

void MethodInvocationExpr::print(){
    cout<<"Method invocation: "<<this->id->id<<" line:" <<this->line << " column : "<<this->column<<endl;
}

void ExpressionStatement::print(){
    cout<<"Expression statement "<<this->line << " column : "<<this->column<<endl;
}

void AssignationStatement::print(){
    cout<<"Assignation statement "<<this->line << " column : "<<this->column<<endl;
}

void WhileStatement::print(){
    cout<<"While statement "<<this->line << " column : "<<this->column<<endl;
}

void ForStatement::print(){
    cout<<"For statement "<<this->line << " column : "<<this->column<<endl;
}

void MainStatement::print(){
    cout<<"Main statement "<<this->id<<" line: "<<this->line << " column : "<<this->column<<endl;
}

void BlockStatement::print(){
    cout<<"Block statement line: "<<this->line << " column : "<<this->column<<endl;
}

void VarDeclarationStatement::print(){
    cout << "Var declaration line: "<<this->line << " column : "<<this->column<<endl;
}

void ConstDeclarationStatement::print(){
    cout << "Const declaration line: "<<this->line << " column : "<<this->column<<endl;
}

void ProcedureDeclarationStatement::print(){
    cout << "Procedure declaration line: "<<this->line << " column : "<<this->column<<endl;
}

void FunctionDeclarationStatement::print(){
    cout << "Function declaration line: "<<this->line << " column : "<<this->column<<endl;
}


PRINT_BINARY_EXPR(And);
PRINT_BINARY_EXPR(Mod);
PRINT_BINARY_EXPR(Div);
PRINT_BINARY_EXPR(Mult);
PRINT_BINARY_EXPR(Or);
PRINT_BINARY_EXPR(Sub);
PRINT_BINARY_EXPR(Add);
PRINT_BINARY_EXPR(Eq);
PRINT_BINARY_EXPR(Neq);
PRINT_BINARY_EXPR(Gt);
PRINT_BINARY_EXPR(Lt);
PRINT_BINARY_EXPR(Gte);
PRINT_BINARY_EXPR(Lte);