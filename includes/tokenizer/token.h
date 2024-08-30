#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <iomanip>
#include "../../includes/tokenizer/constants.h"

using namespace std;

class Token
{
public:
    Token()
    {
        _type = TOKEN_UNKNOWN;
    }
    Token(string str, int type)
    {
        _token = str;
        _type = type;
    }
    friend ostream& operator <<(ostream& outs, const Token& t)
    {
        outs << "|" << t.token_str() << "|";
        return outs;
    }
    int type() const
    {
        return _type;
    }
    string type_string() const
    {
        string type_returned;
        switch (_type)
        {
        case TOKEN_ALPHA:
            type_returned = "ALPHA";
            break;
        case TOKEN_NUMBER:
            type_returned = "DIGIT";
            break;
        case TOKEN_PUNC:
            type_returned = "PUNC";
            break;
        case TOKEN_SPACE:
            type_returned = "SPACE";
            break;
        case TOKEN_OPERATOR:
            type_returned = "OPERATOR";
            break;
        case TOKEN_UNKNOWN: 
            type_returned = "UNKNOWN";
            break;
        case TOKEN_END:
            type_returned = "END";
            break;
        case TOKEN_RESULTSET:
            type_returned = "RESULTSET";
            break;
        case TOKEN_STRING:
            type_returned = "STRING";
        default:
            break;
        }
        return type_returned;
    }
    string token_str() const
    {
        return _token;
    }
private:
    string _token;
    int _type;
};



#endif