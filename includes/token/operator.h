#ifndef OPERATOR_H
#define OPERATOR_H
#include "operator.h"
#include "../../includes/token/token_base.h"
#include <iostream>
#include <iomanip>

using namespace std;

class Operator: public Token_Base{
public:
    Operator(string op):Token_Base(op, 2), _op(op){
        _precedence = get_prec();
    }
    int get_prec(){
        if(_op == "X" || _op == "x")
        {
            return -1;
        }
        else if(_op == "+")
        {
            return 2;
        }
        else if(_op == "-")
        {
            return 2;
        }
        else if(_op == "*")
        {
            return 3;
        }
        else if (_op == "or")
        {
            return 3;
        }
        else if (_op == "and")
        {
            return 4;
        }
        else if (_op == "<")
        {
            return 5;
        }
        else if(_op == "/")
        {
            return 3;
        }
        else if (_op == "=")
        {
            return 5;
        }
        else if (_op == ">")
        {
            return 5;
        }
        else if (_op == "^")
        {
            return 4;
        }
        else if(_op == "sin")
        {
            return 5;
        }
        else if(_op == "cos")
        {
            return 5;
        }
        else if(_op == "tan")
        {
            return 5;
        }
        else if (_op == "sec")
        {
            return 5;
        }
        else if (_op == "csc")
        {
            return 5;
        }
        else if (_op == "cot")
        {
            return 5;
        }
        else if (_op == "abs")
        {
            return 5;
        }
        else if (_op == "log")
        {
            return 6;
        }
        return _precedence;
    }
    string op(){return _op;}

private:
    int _precedence;
    string _op;

};

#endif