#include "sql_parser_functions.h"

//Fill all cells of the array with -1
void init_ptable(int _table[][20])
{
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
             _table[i][j] = -1;
        }
    }
}

//Mark this state (row) with a 1 (success)
void mark_psuccess(int _table[][20], int state)
{
    _table[state][0] = 1;
}

//Mark this state (row) with a 0 (fail)
void mark_pfail(int _table[][20], int state)
{
    _table[state][0] = 0;
}

//true if state is a success state
bool is_psuccess(int _table[][20], int state)
{
    return(_table[state][0] == 1);
}

//Mark a range of cells in the array. 
void mark_pcells(int row, int _table[][20], int from, int to, int state)
{
    for(int i = from; i <= to; i++)
    {
        _table[row][i] = state;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_pcells(int row, int _table[][20], const char columns[], int state)
{
    int from = columns[0];
    int to = columns[strlen(columns)];
    mark_pcells(row, _table, from, to, state);
}

//Mark this row and column
void mark_pcell(int row, int table[][20], int column, int state)
{
    table[row][column] = state;
}

//This can realistically be used on a small table
void print_ptable(int _table[][20])
{
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            cout << _table[i][j] << " ";
        }
        cout << endl;
    }
}