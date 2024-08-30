#include <iostream>
#include <iomanip>
#include <string.h>
#include "stokenize.h"

using namespace std;

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer()
{
    _pos = 0;
    make_table(_table); //Creates a table for stokenizer.
    //print_table(_table);
}

STokenizer::STokenizer(char str[])
{
    make_table(_table); //Makes a table for stokenizer
    //print_table(_table);
    set_string(str); //Sets input str
    _pos = 0; //Sets position to 0.
    //cout << "STR: " << str << endl;
}

bool STokenizer::done()
{
    const bool debug = false;

    if (debug)
    {
        cout << "DEBUG: " << endl;
        cout << "current _pos: " << _pos << ", length: " << strlen(_buffer) << ", char: " << _buffer[_pos] << endl;
    }

    if (_buffer[_pos] == NULL)
    {
        _pos++;
    }

    if (_pos >= strlen(_buffer) + 1)
    {
        return true;
    }

    return false;
}

bool STokenizer::more()
{
    bool return_bool = false;

    if (!done())
    {
        return_bool = true;
    }
    return return_bool;
}

STokenizer& operator >> (STokenizer& s, Token& t)
{
    const bool debug = false;
    string input = "";
    int type = TOKEN_UNKNOWN;    

    s.get_token(0, input);

    int check_me = input.find("\"");

    if (debug)
    {
        cout << "CHECK_ME: " << check_me << endl;

        cout << "INPUT[0]: " << input[0] << endl;

        cout << "INPUT(STOKENIZER >>): " << input << endl;
    }

    if (check_me == 0)
    {
        if (input.size() >= 2 && input.front() == '\"' && input.back() == '\"')
        {
             input = input.substr(1, input.size() - 2);
        }
        //cout << "AFTER QUOTED BOOL INPUT: " << input << endl;
        //cout << "CHICKEN NOOODEL SOOOUPPPPPP" << endl;
    }
    
    if (isalpha(input[0]))
    {
        type = TOKEN_ALPHA;
    }
    else if (isdigit(input[0]))
    {
        type = TOKEN_NUMBER;
    }
    else if (ispunct(input[0]))
    {
        type = TOKEN_PUNC;
    }
    else if (isspace(input[0]))
    {
        type = TOKEN_SPACE;
    }

    t = Token(input, type);
}

void STokenizer::set_string(char str[])
{
    strcpy(_buffer, str);
    _pos = 0;
}

void STokenizer::make_table(int _table[][MAX_COLUMNS]){
        init_table(_table); //Initializes with -1

        //Marks success states
        mark_success(_table, 1);
        mark_success(_table, 2);
        mark_success(_table, 3);
        mark_success(_table, 4);
        mark_success(_table, 5);
        mark_success(_table, 7);
        mark_success(_table, 9);

        //Marks ALFA
        mark_cells(0, _table, ALFA, 2);
        mark_cells(2, _table, ALFA, 2);
  
        //Marks DIGITS
        mark_cells(0, _table, DIGITS, 1);
        mark_cells(1, _table, DIGITS, 1);
        mark_cell(1, _table, 46, 6); //This is a decimal...
        mark_cells(6, _table, DIGITS, 7);
        mark_cells(7, _table, DIGITS, 7);

        //Marks SPACES
        mark_cells(0, _table, SPACES, 3);
        mark_cells(3, _table, SPACES, 3);

        //Marks OPERATORS
        mark_cells(0, _table, OPERATORS, 4);
        mark_cells(4, _table, OPERATORS, 4);

        //Marks PUNC
        mark_cells(0, _table, PUNC, 5);
        mark_cells(5, _table, PUNC, 5);

        //quotes
        mark_cell(0, _table, 34, 8); //quotation
        mark_cells(8, _table, ALFA, 8);
        mark_cells(8, _table, DIGITS, 8);
        mark_cells(8, _table, SPACES, 8);
        mark_cells(8, _table, OPERATORS, 8);
        mark_cells(8, _table, PUNC, 8);
        mark_cell(8, _table, 34, 9);


}

bool STokenizer::get_token(int _state, string& token)
{
    char* current = _buffer + _pos; //Gets current character being looked at
    char* success_state = nullptr;  //Holds success states
    string current_token = "";

    
    if(_table[_state][*current] == -1) //Takes a look at unknown tokens...
    {
        _state = -1; 
        _pos++;
        token = *current;
        return false;
    }

    while(_table[_state][*current] != -1 && *current != '\0')
    {
        current_token += *current;
        _state = _table[_state][*current];
        
        if(is_success(_table, _state)) //Checks for success and keeps track of success state
        { 
            token = current_token;
            success_state = current;
        }
        current++;
        _pos++;
    }

    if(!is_success(_table, _state))
    {
        _state = _table[0][*success_state];
    }

    if(success_state!=nullptr)
    {
        success_state++;
        _pos = success_state-_buffer;
    }

    return (token!="");
}