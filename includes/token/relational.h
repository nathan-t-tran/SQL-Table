#ifndef RELATIONAL_H
#define RELATIONAL_H
#include "operator.h"
#include "token_str.h"
#include <vector>
#include <algorithm>

class Relational: public Operator{
public:
    Relational(string op):Operator(op){
        _op = op;
        _type_Number = 5;
    }

    string get_rel()
    {
        return _op;
    }
    int TypeOf()
    {
        return _type_Number;
    }



private:
    string _op;
    int _type_Number;
};



#endif
