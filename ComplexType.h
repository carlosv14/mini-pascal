#ifndef _COMPLEXTYPE_H_
#include "constants.h"

class ComplexType{
    public:
        ComplexType(PrimitiveType primitiveType, bool isArray){
            this->primitiveType = primitiveType;
            this->isArray = isArray;
        }
        PrimitiveType primitiveType;
        bool isArray;
};

class ArrayType : public ComplexType{
    public:
        ArrayType(int start, int end, PrimitiveType primitiveType)
            : ComplexType(primitiveType, true){
            this->start = start;
            this->end = end;
            this->primitiveType = primitiveType;
        }
        int start, end;
        PrimitiveType primitiveType;
};
#endif