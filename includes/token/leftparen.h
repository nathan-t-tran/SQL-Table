#ifndef LEFTPAREN_H
#define LEFTPAREN_H

#include "../../includes/token/token_base.h"

using namespace std;

class LeftParen: public Token_Base{
public:
    LeftParen():Token_Base("(", 3)
    {
        _leftparen = "(";
    }
    string leftparen(){return _leftparen;}

private:
    string _leftparen;
};

#endif