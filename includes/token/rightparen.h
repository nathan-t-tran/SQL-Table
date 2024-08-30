#ifndef RIGHTPAREN_H
#define RIGHTPAREN_H

#include "../../includes/token/token_base.h"

using namespace std;

class RightParen: public Token_Base{
public:
    RightParen():Token_Base(")", 4)
    {
        _rightparen = ")";
    }
    string rightparen(){return _rightparen;}
private:
    string _rightparen;
};

#endif