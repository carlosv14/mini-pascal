#include "ast.h"
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include "asm.h"

extern asmcode assemblyResult;

using namespace std;

int globalStackPointer = 0;

class CodeGenerationVarInfo{
    public:
        CodeGenerationVarInfo(bool isParameter, ComplexType * type, int offset){
            this->isParameter = isParameter;
            this->offset = offset;
            this->type = type;
        }
        bool isParameter;
        ComplexType * type;
        int offset;
};

map<string, MethodInformation *> methods = {};
map<string, CodeGenerationVarInfo *> codeGenerationVars = {};

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
    this->trueStatement->print();
    if (this->falseStatement != NULL)
    {
        this->falseStatement->print();
    }
    
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
    this->stmt->print();
}

void ForStatement::print(){
    cout<<"For statement "<<this->line << " column : "<<this->column<<endl;
    this->stmt->print();
}

void MainStatement::print(){
    cout<<"Main statement "<<this->id<<" line: "<<this->line << " column : "<<this->column<<endl;
    this->stmt->print();
}

void BlockStatement::print(){
    cout<<"Block statement line: "<<this->line << " column : "<<this->column<<endl;
    list<DeclarationStatement *>::iterator it = this->declarations->begin();
    while (it != this->declarations->end())
    {
        (*it)->print();
        it++;
    }

     list<Statement *>::iterator itStmt = this->stmts->begin();
    while (itStmt != this->stmts->end())
    {
        (*itStmt)->print();
        itStmt++;
    }
    
}

void VarDeclarationStatement::print(){
    cout << "Var declaration line: "<<this->line << " column : "<<this->column<<endl;
}

void ConstDeclarationStatement::print(){
    cout << "Const declaration line: "<<this->line << " column : "<<this->column<<endl;
}

void ProcedureDeclarationStatement::print(){
    cout << "Procedure declaration line: "<<this->line << " column : "<<this->column<<endl;
    this->varDeclaration->print();
    this->statement->print();
}

void FunctionDeclarationStatement::print(){
    cout << "Function declaration line: "<<this->line << " column : "<<this->column<<endl;
    this->varDeclaration->print();
    this->statement->print();
}





/* Expression Get Type*/

PrimitiveType StringExpr::getType(){
    return STRING;
}

PrimitiveType CharExpr::getType(){
    return CHAR;
}

PrimitiveType FloatExpr::getType(){
    return REAL;
}

PrimitiveType IntExpr::getType(){
    return INTEGER;
}

PrimitiveType BoolExpr::getType(){
    return BOOLEAN;
}

PrimitiveType UnaryExpr::getType(){
    PrimitiveType type = this->expr->getType();
    if (this->op == NOT && type != BOOLEAN)
    {
        cerr<<"No se puede aplicar el operador NOT a una expresion no booleana"<<endl;
    }
    return type;
}


map<string, PrimitiveType> exprResultTypes = {
    {"INTEGER,INTEGER", INTEGER},
    {"INTEGER,REAL", REAL},
    {"REAL,REAL", REAL},
    {"REAL,INTEGER", REAL},
    {"STRING,STRING", STRING},
    {"STRING,CHAR", STRING},
    {"CHAR,STRING", STRING},
    {"BOOLEAN,BOOLEAN", BOOLEAN},
};

string getTypeAsString(PrimitiveType type){
    if (type == STRING)
    {
        return "STRING";
    }else if(type == CHAR){
        return "CHAR";
    }else if(type == BOOLEAN){
        return "BOOLEAN";
    }else if(type == INTEGER){
        return "INTEGER";
    }
    else if(type == REAL){
        return "REAL";
    }
    return "NONE";
}

#define GET_TYPE_BINARY_EXPR(name)\
PrimitiveType name##Expr::getType(){\
    string leftType = getTypeAsString(this->left->getType());\
    string rightType = getTypeAsString(this->right->getType());\
    PrimitiveType type = exprResultTypes[leftType+","+rightType];\
    if(type == 0){\
        cerr<<"El tipo de dato "<<leftType<<" no puede ser operado con el tipo de dato "<<rightType<<" linea: "<<this->line<<" columna: "<<this->column<<endl;\
    }\
    return type;\
}\

#define GET_TYPE_BINARY_EXPR_BOOL(name)\
PrimitiveType name##Expr::getType(){\
    string leftType = getTypeAsString(this->left->getType());\
    string rightType = getTypeAsString(this->right->getType());\
    if(leftType != rightType){\
        cerr<<"El tipo de dato "<<leftType<<" no puede ser operado con el tipo de dato "<<rightType<<" linea: "<<this->line<<" columna: "<<this->column<<endl;\
    }\
    return BOOLEAN;\
}\


class Context{
    public:
        struct Context * prev;
        map<string, PrimitiveType> vars;
};

map<string, PrimitiveType> vars;
map<string, PrimitiveType> globalVars;
Context * currentContext = NULL;

void pushContext(){
    vars.clear();
    Context * context = new Context();
    context->vars = vars;
    context->prev = currentContext;
    currentContext = context;
}

void popContext(){
    if (currentContext != NULL)
    {
        Context * previous = currentContext->prev;
        free(currentContext);
        currentContext = previous;
    }
}

PrimitiveType getLocalVarType(string id){
    Context * context = currentContext;
    while (context  != NULL)
    {
        if (context->vars[id] != 0)
        {
            return context->vars[id];
        }
        context = context->prev;
    }
    return NONE;
}

PrimitiveType getVarType(string id){
    PrimitiveType resultType = NONE;
    if (currentContext != NULL)
    {
        resultType = getLocalVarType(id);
    }
    if (resultType != NONE)
    {
        return resultType;
    }
    return globalVars[id] != 0 ? globalVars[id] : NONE;
}

PrimitiveType MethodInvocationExpr::getType(){
    MethodInformation * method = methods[this->id->id];
    if (method == NULL)
    {
        cerr<<this->id->id<<" no declarado linea: "<<this->line<<" column "<<this->column<<endl;
        return NONE;
    }
    
    if (method->parameters->ids->size() > this->args->size())
    {
        cerr<<"Muy pocos argumentos para el método: "<<this->id->id <<" linea: "<<this->line<<" column "<<this->column<<endl;
        return NONE;
    }
    
    if (method->parameters->ids->size() < this->args->size())
    {
        cerr<<"Muchos argumentos para el método: "<<this->id->id <<" linea: "<<this->line<<" column "<<this->column<<endl;
        return NONE;
    }

    ComplexType * paramsType = method->parameters->type;
    list<Expression*>::iterator argsIt = this->args->begin();
    while (argsIt != this->args->end())
    {
        if ((*argsIt)->getType() != paramsType->primitiveType)
        {   
            cerr<<"Tipo de dato incorrecto en parámetros, se esperaba tipo: "<<getTypeAsString(paramsType->primitiveType)<<" se obtuvo: "<< getTypeAsString((*argsIt)->getType()) <<" linea: "<<this->line<<" column "<<this->column<<endl;
            return NONE;
        }
        argsIt++;
    }
    
    return method->returnType;
}

PrimitiveType IdExpr::getType(){
    PrimitiveType type = getVarType(this->id);
    if (type == NONE)
    {
        cerr<<this->id<<" no declarada linea: "<<this->line<<" column "<<this->column<<endl;
    }
    
    return type;
}

PrimitiveType ArrayExpr::getType(){
    return this->id->getType();
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

GET_TYPE_BINARY_EXPR(And);
GET_TYPE_BINARY_EXPR(Mod);
GET_TYPE_BINARY_EXPR(Div);
GET_TYPE_BINARY_EXPR(Mult);
GET_TYPE_BINARY_EXPR(Or);
GET_TYPE_BINARY_EXPR(Sub);
GET_TYPE_BINARY_EXPR(Add);
GET_TYPE_BINARY_EXPR_BOOL(Eq);
GET_TYPE_BINARY_EXPR_BOOL(Neq);
GET_TYPE_BINARY_EXPR_BOOL(Gt);
GET_TYPE_BINARY_EXPR_BOOL(Lt);
GET_TYPE_BINARY_EXPR_BOOL(Gte);
GET_TYPE_BINARY_EXPR_BOOL(Lte);

/* End of expression Get Type*/

/* Begin statement evaluate semantic*/

void WriteStatement::evaluateSemantic(){
    list<Expression *>::iterator it = this->expressions->begin();
    while (it != this->expressions->end())
    {
        PrimitiveType type = (*it)->getType();
        if(type != STRING && type != INTEGER && type != CHAR && type != REAL){
            cerr<<"No se permite imprimir tipos distintos de string, integer, char y real linea:"<<this->line <<" columna: "<<this->column<<endl;
            return;
        }
        it++;
    }
}

void ReadStatement::evaluateSemantic(){
    list<Expression *>::iterator it = this->expressions->begin();
    while (it != this->expressions->end())
    {
        PrimitiveType type = (*it)->getType();
        if(type != STRING && type != INTEGER && type != CHAR && type != REAL && type != BOOLEAN){
            cerr<<"No se permite leer tipos distintos de string, integer, char, booleano y real linea:"<<this->line <<" columna: "<<this->column<<endl;
            return;
        }
        it++;
    }
}

void IfStatement::evaluateSemantic(){
    if (this->expression->getType() != BOOLEAN)
    {
        cerr<<"El if requiere una expresion booleana linea: "<< this->line <<" columna "<<this->column<<endl;
        return;
    }
    pushContext();
    this->trueStatement->evaluateSemantic();
    popContext();
    if (this->falseStatement != NULL)
    {
       this->falseStatement->evaluateSemantic();
    }
}

void ExpressionStatement::evaluateSemantic(){
    this->expr->getType();
}

void AssignationStatement::evaluateSemantic(){
    if (this->isArray && this->index->getType() != INTEGER)
    {
        cerr<<"El indice en el arreglo debe ser un entero linea: "<<this->line<<" columna: "<<this->column<<endl;
        return;
    }
    
    PrimitiveType varType = getVarType(this->id);
    PrimitiveType exprType = this->expr->getType();
    if (varType != exprType)
    {
        cerr<<"No se puede assignar "<< getTypeAsString(exprType)<< " a la variable de tipo "<<getTypeAsString(varType)<<" linea " <<this->line<<" columna: "<<this->column<<endl;
        return;
    }
}


void WhileStatement::evaluateSemantic(){
    if (this->expression->getType() != BOOLEAN)
    {
        cerr<<"El while requiere una expresion booleana linea: "<< this->line <<" columna "<<this->column<<endl;
        return;
    }
    pushContext();
    this->stmt->evaluateSemantic();
    popContext();
}

void ForStatement::evaluateSemantic(){    
    PrimitiveType varType = getVarType(this->id);
    PrimitiveType fromExprType = this->fromExpr->getType();
    PrimitiveType toExprType = this->toExpr->getType();
    if (varType != fromExprType)
    {
        cerr<<"No se puede assignar "<< getTypeAsString(fromExprType)<< " a la variable de tipo "<<getTypeAsString(varType) << " linea " <<this->line<<" columna: "<<this->column<<endl;
        return;
    }

      if (varType != toExprType)
    {
        cerr<<"No se puede assignar "<< getTypeAsString(toExprType)<< " a la variable de tipo "<<getTypeAsString(varType) << " linea " <<this->line<<" columna: "<<this->column<<endl;
        return;
    }

    pushContext();
    this->stmt->evaluateSemantic();
    popContext();
}

void MainStatement::evaluateSemantic(){
    this->stmt->evaluateSemantic();
}

void VarDeclarationStatement::evaluateSemantic(){
   list<string>::iterator it = this->ids->begin();
   while (it != this->ids->end())
   {
        if (getVarType(*it) != NONE)
        {
            cerr<<"Ya existe una variable con el nombre "<< (*it)<<" linea " <<this->line<<" columna: "<<this->column<<endl;
            return;
        }
        ArrayType * arrayType = static_cast<ArrayType *>(this->type);
        if (this->type->isArray && arrayType->end <= 0)
        {
            cerr<<"El fin de un arreglo debe ser mayor a 0 linea " <<this->line<<" columna: "<<this->column<<endl;
            return;
        }
        
        currentContext->vars[(*it)] = this->type->primitiveType;        
        it++;
   }
}

void ConstDeclarationStatement::evaluateSemantic(){
    if (getVarType(this->id) != NONE)
    {
        cerr<<"Ya existe una variable con el nombre "<< (this->id)<<" linea " <<this->line<<" columna: "<<this->column<<endl;
        return;
    }
   ArrayType * arrayType = static_cast<ArrayType *>(this->type);
   if (arrayType->isArray)
   {
        cerr<<"No puede existir una constante de tipo arreglo linea " <<this->line<<" columna: "<<this->column<<endl;
        return;
   }
   
   currentContext->vars[this->id] = this->type->primitiveType;
}

void ProcedureDeclarationStatement::evaluateSemantic(){
    if(this->varDeclaration->ids->size() > 4){
        cerr<<"Solo se soportan 4 parametros por método, método "<<this->id<<" linea "<<this->line<<" columna "<<this->column<<endl;
        return;
    }
    if (methods[this->id] != NULL)
    {
        cerr<<"Ya existe un método con el nombre"<<this->id<<" linea "<<this->line<<" columna "<<this->column<<endl;
        return;
    }
    methods[this->id] = new MethodInformation(VOID, this->varDeclaration);
    pushContext();
    this->varDeclaration->evaluateSemantic();
    this->statement->evaluateSemantic();
    popContext();
}

void FunctionDeclarationStatement::evaluateSemantic(){
    if(this->varDeclaration->ids->size() > 4){
        cerr<<"Solo se soportan 4 parametros por método, método "<<this->id<<" linea "<<this->line<<" columna "<<this->column<<endl;
        return;
    }
    if (methods[this->id] != NULL)
    {
        cerr<<"Ya existe un método con el nombre"<<this->id<<" linea "<<this->line<<" columna "<<this->column<<endl;
        return;
    }

    methods[this->id] = new MethodInformation(this->type->primitiveType, this->varDeclaration);
    pushContext();
    this->varDeclaration->evaluateSemantic();
    this->statement->evaluateSemantic();
    popContext();
}

void BlockStatement::evaluateSemantic(){
    pushContext();
    list<DeclarationStatement *>::iterator itDec = this->declarations->begin();
    while (itDec != this->declarations->end())
    {
        (*itDec)->evaluateSemantic();
        itDec++;
    }
    list<Statement *>::iterator itStmt = this->stmts->begin();
    while (itStmt != this->stmts->end())
    {
        (*itStmt)->evaluateSemantic();
        itStmt++;
    }
    popContext();
}

// end get type

// begin code gen

const char * intTemps[]{ "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9" };
set<string> intTempMap;
#define INT_TEMP_COUNT 10

const char * floatTemps[]{ "$f0", "$f1", "$f2", "$f3", "$f4", "$f5", "$f6", "$f7", "$f8", "$f9", "$f10", "$f11", "$f12", "$f13", "$f14", "$f15", "$f16", "$f17", "$f18", "$f19", "$f20", "$f21", "$f22", "$f23", "$f24", "$f25", "$f26", "$f27", "$f28", "$f29", "$f30", "$f31" };
set<string> floatTempMap;
#define FLOAT_TEMP_COUNT 32

string getIntTemp(){
    for (int i = 0; i < INT_TEMP_COUNT; i++)
    {
        if (intTempMap.find(intTemps[i]) == intTempMap.end())
        {
            intTempMap.insert(intTemps[i]);
            return string(intTemps[i]);
        }
    }
    cerr<<"No hay registros temporales disponibles"<<endl;
    exit(0);
}

string getFloatTemp(){
    for (int i = 0; i < FLOAT_TEMP_COUNT; i++)
    {
        if (floatTempMap.find(floatTemps[i]) == floatTempMap.end())
        {
            floatTempMap.insert(floatTemps[i]);
            return string(floatTemps[i]);
        }
    }
    cerr<<"No hay registros temporales disponibles"<<endl;
    exit(0);
}

void releaseRegister(string temp){
    intTempMap.erase(temp);
    floatTempMap.erase(temp);
}

int labelCounter = 0;
string newLabel(string prefix){
    stringstream label;
    label <<prefix<<"_"<<labelCounter;
    labelCounter++;
    return label.str();
}

/*
    print("hola");
    

    string_0: .asciiz "hola"

    la $a0, string_0
*/

void StringExpr::generateCode(CodeContext &context){
    string label = newLabel("string");
    stringstream code;
    code << label<<": .asciiz "<< this->value<<endl;
    context.code = "";
    context.place = label;
    context.type = new ComplexType((PrimitiveType)STRING, false);
    string finalCode = code.str();
    replace(finalCode.begin(), finalCode.end(), '\'', '\"');
    assemblyResult.data += finalCode;
}


void IntExpr::generateCode(CodeContext &context){
    stringstream code;
    string temp = getIntTemp();
    code<<"li "<<temp<<", "<< this->value<<endl;
    context.code = code.str();
    context.place = temp;
    context.type = new ComplexType((PrimitiveType)INTEGER, false);
}

void FloatExpr::generateCode(CodeContext &context){
    stringstream code;
    string temp = getFloatTemp();
    code<<"li.s "<<temp<<", "<< this->value<<endl;
    context.code = code.str();
    context.place = temp;
    context.type = new ComplexType((PrimitiveType)REAL, false);
}

void IdExpr::generateCode(CodeContext &context){
    if (codeGenerationVars.find(this->id) == codeGenerationVars.end())
    {
        context.type = new ComplexType(globalVars[this->id], false);
        if (globalVars[this->id] == NONE)
        {
            string temp = getIntTemp();
            context.code = "la " + temp +", "+ this->id + "\n";
            context.place = temp;
        }
        
        if (globalVars[this->id] == REAL)
        {
            string floatTemp = getFloatTemp();
            context.place = floatTemp;
            context.code = "l.s "+floatTemp + ", " + this->id +"\n";
        }else if (globalVars[this->id] == INTEGER){
            string intTemp = getIntTemp();
            context.place = intTemp;
            context.code = "lw "+intTemp + ", " + this->id +"\n";
        }
    }else{
        context.type = codeGenerationVars[this->id]->type;
        if (codeGenerationVars[this->id]->type->primitiveType == REAL && !codeGenerationVars[this->id]->type->isArray)
        {
            string floatTemp = getFloatTemp();
            context.place = floatTemp;
            context.code = "l.s "+floatTemp + ", " + to_string(codeGenerationVars[this->id]->offset) +"($sp)\n";
        }
        else if (codeGenerationVars[this->id]->type->primitiveType == INTEGER && !codeGenerationVars[this->id]->type->isArray){
            string intTemp = getIntTemp();
            context.place = intTemp;
            context.code = "lw "+intTemp + ", " + to_string(codeGenerationVars[this->id]->offset) +"($sp)\n";
        }else if(codeGenerationVars[this->id]->type->isArray){
          string intTemp = getIntTemp();
          context.code = "la "+ intTemp + to_string(codeGenerationVars[this->id]->offset) +"($sp)\n";
          context.place = intTemp;
        }
    }
    
}

// a[2]
void ArrayExpr::generateCode(CodeContext &context){
    CodeContext indexCode;
    stringstream code;
    this->index->generateCode(indexCode);
    releaseRegister(indexCode.place);

    string temp = getIntTemp();
    string address = getIntTemp();
    code<< indexCode.code <<endl
    <<"li $a0, 4"<<endl
    <<"mult $a0, "<<indexCode.place<<endl
    <<"mflo "<< temp<< endl;

    if (codeGenerationVars.find(this->id->id) == codeGenerationVars.end())
    {
        code <<"la "<<address<<", "<<this->id->id<<endl
        <<"add "<<temp<<", "<<address<<", "<<temp<<endl;
        releaseRegister(address);
        if (globalVars[this->id->id] == INTEGER)
        {
            code<< "lw "<< temp<<", 0("<<temp<<")"<<endl;
            context.place = temp;
            context.type = new ComplexType(INTEGER, false);
        }else{
            string value = getFloatTemp();
            code<< "l.s "<< value<<", 0("<<temp<<")"<<endl;
            context.place = value;
            context.type = new ComplexType(REAL, false);
            releaseRegister(temp);
        }
    }else{
        if(!codeGenerationVars[this->id->id]->isParameter){
            code<<"la "<<address<<", "<<codeGenerationVars[this->id->id]->offset<<"($sp)"<<endl;
        }else{
            code<<"lw "<<address<<", "<<codeGenerationVars[this->id->id]->offset<<"($sp)"<<endl;
        }

        code<<"add "<<temp<<", "<<address<<", "<<temp<<endl;
        if (codeGenerationVars[this->id->id]->type->primitiveType == INTEGER)
        {
            code<<"lw "<<temp<<", 0("<<temp<<")"<<endl;
            context.place = temp;
            context.type = codeGenerationVars[this->id->id]->type;
        }else{
            string value = getFloatTemp();
            code<<"l.s "<<value<<", 0("<<temp<<")"<<endl;
            context.place = value;
            context.type = codeGenerationVars[this->id->id]->type;
            releaseRegister(temp);
        }
    }
    context.code = code.str();
}

void CharExpr::generateCode(CodeContext &context){
    stringstream code;
    string temp = getIntTemp();
    code<<"li "<<temp<<", '"<< this->value<<"'"<<endl;
    context.code = code.str();
    context.place = temp;
    context.type = new ComplexType((PrimitiveType)CHAR, false);
}

void BoolExpr::generateCode(CodeContext &context){
    
}

void UnaryExpr::generateCode(CodeContext &context){
    
}

void MethodInvocationExpr::generateCode(CodeContext &context){
    
}

#define GEN_CODE_BINARY_EXPR(name)\
void name##Expr::generateCode(CodeContext &context){\
};

string intArithmetic(CodeContext &leftCode, CodeContext &rightCode, CodeContext &resultCode, char op){
    resultCode.place = getIntTemp();
    stringstream code;
    switch (op)
    {
    case '+':
        code<<"add "<<resultCode.place<<", "<< leftCode.place<<", "<<rightCode.place<<endl;
        break;
     case '-':
        code<<"sub "<<resultCode.place<<", "<< leftCode.place<<", "<<rightCode.place<<endl;
        break;
    case '*':
        code<<"mult "<< leftCode.place<<", "<<rightCode.place<<endl<< "mflo "<<resultCode.place;
        break;
    case '/':
        code<<"div "<< leftCode.place<<", "<<rightCode.place<<endl<< "mflo "<<resultCode.place;
        break;
    default:
        break;
    }
    return code.str();
}

string floatArithmetic(CodeContext &leftCode, CodeContext &rightCode, CodeContext &resultCode, char op){
    resultCode.place = getFloatTemp();
    stringstream code;
    switch (op)
    {
    case '+':
        code<<"add.s "<<resultCode.place<<", "<< leftCode.place<<", "<<rightCode.place<<endl;
        break;
     case '-':
        code<<"sub.s "<<resultCode.place<<", "<< leftCode.place<<", "<<rightCode.place<<endl;
        break;
    case '*':
        code<<"mul.s "<<resultCode.place<<", "<< leftCode.place<<", "<<rightCode.place<<endl;
        break;
    case '/':
        code<<"div.s "<<resultCode.place<<", "<< leftCode.place<<", "<<rightCode.place<<endl;
        break;
    default:
        break;
    }
    return code.str();
}

void toReal(CodeContext &context){
    string floatTemp = getFloatTemp();
    stringstream code;
    code << context.code
    << "mtc1 "<<context.place<<", "<<floatTemp<<endl
    << "cvt.s.w "<< floatTemp<<", "<<floatTemp<<endl;
    releaseRegister(context.place);
    context.place = floatTemp;
    context.type = new ComplexType(REAL, false);
    context.code = code.str();
}

#define GEN_ARIT_CODE_BINARY_EXPR(name, op)\
void name##Expr::generateCode(CodeContext &context){\
    CodeContext leftCode, rightCode;\
    stringstream code;\
    this->left->generateCode(leftCode);\
    this->right->generateCode(rightCode);\
    if (leftCode.type->primitiveType == INTEGER && rightCode.type->primitiveType == INTEGER)\
    {\
        context.type = leftCode.type;\
        releaseRegister(leftCode.place);\
        releaseRegister(rightCode.place);\
        code<< leftCode.code<<endl\
        <<rightCode.code<<endl\
        << intArithmetic(leftCode, rightCode, context, op)<<endl;\
    }\
    else{\
        context.type = new ComplexType(REAL, false);\
        if(leftCode.type->primitiveType != REAL)\
            toReal(leftCode);\
        if(rightCode.type->primitiveType != REAL)\
            toReal(rightCode);\
        releaseRegister(leftCode.place);\
        releaseRegister(rightCode.place);\
        code<< leftCode.code<<endl\
        <<rightCode.code<<endl\
        << floatArithmetic(leftCode, rightCode, context, op)<<endl;\
    }\
    context.code = code.str();\
}\

GEN_CODE_BINARY_EXPR(And);
GEN_ARIT_CODE_BINARY_EXPR(Mod, '%');
GEN_ARIT_CODE_BINARY_EXPR(Div, '/');
GEN_ARIT_CODE_BINARY_EXPR(Mult, '*');
GEN_CODE_BINARY_EXPR(Or);
GEN_ARIT_CODE_BINARY_EXPR(Sub, '-');
GEN_ARIT_CODE_BINARY_EXPR(Add, '+');
GEN_CODE_BINARY_EXPR(Eq);
GEN_CODE_BINARY_EXPR(Neq);
GEN_CODE_BINARY_EXPR(Gt);
GEN_CODE_BINARY_EXPR(Gte);
GEN_CODE_BINARY_EXPR(Lte);

void LtExpr::generateCode(CodeContext &context){
    CodeContext leftCode, rightCode;
    stringstream code;
    this->left->generateCode(leftCode);
    this->right->generateCode(rightCode);
    if (leftCode.type->primitiveType == INTEGER && rightCode.type->primitiveType == INTEGER)
    {
        context.type = leftCode.type;
        code<<leftCode.code<<endl
        <<rightCode.code<<endl;
        releaseRegister(leftCode.place);
        releaseRegister(rightCode.place);
        string temp = getIntTemp();
        code<<"slt "<<temp<<", "<<leftCode.place<<", "<<rightCode.place<<endl;
        context.place = temp;
    }else{
        context.type = leftCode.type;
        code<<leftCode.code<<endl
        <<rightCode.code<<endl;
        releaseRegister(leftCode.place);
        releaseRegister(rightCode.place);
        string temp = getIntTemp();
        code<<"c.lt.s "<<leftCode.place<<", "<<rightCode.place<<endl;
        context.place = temp;
    }
    context.code= code.str();
}

string WriteStatement::generateCode(){
    list<Expression*>::iterator it = this->expressions->begin();
    stringstream code;
    while (it != this->expressions->end())
    {
        CodeContext exprContext;
        (*it)->generateCode(exprContext);
        releaseRegister(exprContext.place);
        code << exprContext.code<<endl;
        if (exprContext.type->primitiveType == INTEGER)
        {
            code<<"move $a0, "<<exprContext.place<<endl
            <<"li $v0, 1"<<endl;
        }else if (exprContext.type->primitiveType == REAL)
        {   
            code<<"mov.s $f12, "<<exprContext.place<<endl
            << "li $v0, 2"<<endl;
        }else if(exprContext.type->primitiveType == STRING){
            code<<"la $a0, "<<exprContext.place<<endl
            <<"li $v0, 4"<<endl;
        }else if(exprContext.type->primitiveType == CHAR){
            code<<"move $a0, "<<exprContext.place<<endl
            <<"li $v0, 11"<<endl;
        }
        
        code<<"syscall"<<endl;
        it++;
    }
    code <<"la $a0, nextline"<<endl
        <<"li $v0, 4"<<endl
        <<"syscall"<<endl;
    return code.str();
}

string ReadStatement::generateCode(){
    list<Expression*>::iterator it = this->expressions->begin();
    stringstream code;
    while (it != this->expressions->end())
    {
        CodeContext exprContext;
        (*it)->generateCode(exprContext);
        releaseRegister(exprContext.place);
        code << exprContext.code<<endl;
        if (exprContext.type->primitiveType == INTEGER)
        {
            code<<"li $v0, 5"<<endl
            <<"syscall"<<endl
            << "move "<<exprContext.place<<", $v0";
        }else if (exprContext.type->primitiveType == REAL)
        {   
            code<<"li $v0, 6"<<endl
            <<"syscall"<<endl
            << "mov.s "<<exprContext.place<<", $f0";
        }else if(exprContext.type->primitiveType == STRING){
           /*
            TODOOOO.
           */
        }else if(exprContext.type->primitiveType == CHAR){
           code<<"li $v0, 12"<<endl
            <<"syscall"<<endl
            << "move "<<exprContext.place<<", $v0";
        }
        it++;
    }
    
    return code.str();
}

string IfStatement::generateCode(){
    return "";
}

string ExpressionStatement::generateCode(){
    return "";
}

string AssignationStatement::generateCode(){
    CodeContext rightSideCode;
    stringstream code;
    this->expr->generateCode(rightSideCode);
    code << rightSideCode.code;
    if (codeGenerationVars.find(this->id) == codeGenerationVars.end())
    {
        if (rightSideCode.type->primitiveType == INTEGER && !rightSideCode.type->isArray)
        {
            code<<"sw "<<rightSideCode.place<<", "<<this->id<<endl;
        }
        else if (rightSideCode.type->primitiveType == REAL && !rightSideCode.type->isArray)
        {
            code<<"s.s "<<rightSideCode.place<<", "<<this->id<<endl;
        }
    }else{
        if(this->isArray){
            CodeContext indexCode;
            this->index->generateCode(indexCode);
            string temp = getIntTemp();
            string address = getIntTemp();
            releaseRegister(indexCode.place);
            code<< indexCode.code <<endl
            <<"li $a0, 4"<<endl
            <<"mult $a0, "<<indexCode.place<<endl
            <<"mflo "<< temp<<endl;
            if (!codeGenerationVars[this->id]->isParameter)
            {
                code<<"la "<<address<<", "<<codeGenerationVars[this->id]->offset<<"($sp)"<<endl;
            }else{
                code<<"lw "<<address<<", "<<codeGenerationVars[this->id]->offset<<"($sp)"<<endl;
            }
            code<<"add "<<temp<<", "<<temp<<", "<<address<<endl;
            code<<rightSideCode.code<<endl
            <<"sw "<<rightSideCode.place<<", 0("<<temp<<")"<<endl;
            releaseRegister(temp);
            releaseRegister(address);
            releaseRegister(indexCode.place);
        }
        else if (rightSideCode.type->primitiveType == INTEGER)
        {
            code<<"sw "<<rightSideCode.place<<", "<<codeGenerationVars[this->id]->offset<<"($sp)"<<endl;
        }
        else if (rightSideCode.type->primitiveType == REAL)
        {
            code<<"s.s "<<rightSideCode.place<<", "<<codeGenerationVars[this->id]->offset<<"($sp)"<<endl;
        }
    }
    releaseRegister(rightSideCode.place);
    return code.str();
}

string WhileStatement::generateCode(){
    stringstream code;
    string whileLabel = newLabel("while");
    string endWhile = newLabel("endWhile");
    CodeContext exprCode;
    this->expression->generateCode(exprCode);
    releaseRegister(exprCode.place);
    code<< whileLabel<<": "<<endl
    << exprCode.code << endl;
    if (exprCode.type->primitiveType == INTEGER)
    {
        code<<"beqz "<<exprCode.place<<", "<<endWhile<<endl;
    }else{
        code<<"bc1f "<<endWhile<<endl;
    }
    code<<this->stmt->generateCode()<<endl
    <<"j "<<whileLabel<<endl
    <<endWhile<<": "<<endl;
    
    return code.str();
}

string ForStatement::generateCode(){
    return "";
}

string MainStatement::generateCode(){
    stringstream code;
    code<<"main: "<<endl
    <<this->stmt->generateCode()<<endl
    <<"li $v0, 10"<<endl
    <<"syscall"<<endl;
    return code.str();
}

string VarDeclarationStatement::generateCode(){
    list<string>::iterator idIt = this->ids->begin();
    while (idIt != this->ids->end())
    {
        codeGenerationVars[(*idIt)] = new CodeGenerationVarInfo(false, this->type, globalStackPointer);
        if (!this->type->isArray)
        {
            globalStackPointer+=4;
        }else{
            ArrayType * arrayType = ((ArrayType *)this->type);
            int size = 0;
            if (arrayType->start > 1)
            {
                size = arrayType->end - arrayType->start;
            }else{
                size = arrayType->end;
            }
            globalStackPointer += size * 4;
        }
        idIt++;
    }
    return "";
}

string ConstDeclarationStatement::generateCode(){
    return "";
}

string ProcedureDeclarationStatement::generateCode(){
    return "";
}

string FunctionDeclarationStatement::generateCode(){
    return "";
}

string BlockStatement::generateCode(){
    stringstream code;
    list<DeclarationStatement*>::iterator declIt = this->declarations->begin();
    while (declIt != this->declarations->end())
    {
        code<<(*declIt)->generateCode();
        declIt++;
    }
    

    list<Statement *>::iterator it = this->stmts->begin();
    while (it != this->stmts->end())
    {
        code<<(*it)->generateCode();
        it++;
    }
    
    return code.str();
}
// end code gen