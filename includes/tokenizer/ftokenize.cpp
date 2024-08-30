#include <iostream>
#include <iomanip>
#include "ftokenize.h"
#include "../../includes/tokenizer/stokenize.h"

using namespace std;

FTokenizer::FTokenizer(char* fname)
{
    _f.open(fname); //Opens new file.
    if (_f.fail())
    {
        cout << "Failed to open file: " << fname << endl;
        exit(0);
    }
    _blockPos = 0;
    _pos = 0;
    get_new_block(); //Gets new block.
}
Token FTokenizer::next_token()
{
    
}
bool FTokenizer::more()       //returns the current value of _more
{
    if(!_f.eof() == false) //Checks if end of file is false
    {
        _more = true;
    }
    else if(_f.eof() == true && _stk.more() == true) //Check if end of file is true, but there is more tokens.
    {
        _more = true;
    }
    else if(_f.eof() == true && _stk.more() == false) //Checks if end of file is true, but there are no more tokens.
    {
        _more = false;
    }
    return _more;
}
int FTokenizer::pos()          //returns the value of _pos
{
    return _pos;
}
int FTokenizer::block_pos()     //returns the value of _blockPos
{
    return _blockPos;
}
FTokenizer& operator >> (FTokenizer& f, Token& t)
{
    
}
bool FTokenizer::get_new_block()
{
    char buffer[500];

    _f.read(buffer,MAX_BLOCK);

    buffer[_f.gcount()] = '\0';

    _stk.set_string(buffer);

    return true;
}