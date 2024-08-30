#ifndef LOGICAL_H
#define LOGICAL_H
#include <algorithm>
#include "operator.h"
#include "resultset.h"
#include "../../includes/bplustree/map.h"
#include "../../includes/bplustree/multimap.h"
#include "../../includes/token/token_base.h"

class Logical: public Operator{
public:
    Logical(string op):Operator(op){
        _op = op;
        _type_Number = 4;
    }

    string get_log()
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