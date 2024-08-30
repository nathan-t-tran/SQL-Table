#ifndef TOKEN_STR_H
#define TOKEN_STR_H
#include "../../includes/token/token_base.h"
#include "../../includes/tokenizer/constants.h"
#include <string>


class TokenStr: public Token_Base{
public:
    TokenStr():Token_Base("", TOKEN_STRING){
        _str = "";
        _type = 0;
    }
    TokenStr(string entry):Token_Base(entry, TOKEN_STRING){
        _str = entry;
        _type = 0;
    }

    string str(){ //access string
        return _str;
    }
private:
    string _str;
    int _type;
};




#endif