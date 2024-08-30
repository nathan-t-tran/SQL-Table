#ifndef RESULTSET_H
#define RESULTSET_H
#include <vector>
#include <iomanip>
#include<iostream>
#include "../../includes/token/operator.h"

class ResultSet: public Token_Base{
public:
    ResultSet(vector<long> inputed_record):Token_Base("RESULT", TOKEN_RESULTSET)
    {
        //cout << "I AM HAPPENING IN RESULTSET" << endl;
        _type_Number = 6; 
        record_number = inputed_record;
             
    }

    vector<long> get_recnos()
    {
        //cout << "I AM HAPPENING IN RECORD NUM!" << endl;
        //cout << "RECORD NUMBER: " << record_number << endl;
        return record_number;
    }

private:
    int _type_Number;
    vector<long> record_number;
};



#endif