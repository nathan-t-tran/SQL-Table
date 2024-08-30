#include <iostream>
#include <iomanip>
#include "state_machine_functions.h"

using namespace std;

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS])
{
    for (int row = 0; row < MAX_ROWS; row++)
    {
        for (int col = 0; col < MAX_COLUMNS; col++)
        {
            _table[row][col] = -1;
        }
    }
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 1; //Marks the row with a 1.
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 0;  //Marks row with a 0.
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state)
{
    bool success_bool = false; //Sets a bool val to return as false

    if (_table[state][0] == 1) //Checks for success state
    {
        success_bool = true; //Changes bool to true if success state is found
    }

    return success_bool; //Returns bool val
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state)
{
    for (int cols = from; cols < to; cols++)
    {
        _table[row][cols] = state;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state)
{
    int inc = 0;

    while (columns[inc] != NULL)
    {
        _table[row][columns[inc]] = state;
        inc++;
    }
}

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state)
{
    table[row][column] = state; //Marks given cell at specified row and col w state.
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS])
{
    for (int row = 0; row < MAX_ROWS; row++) //Loops through rows
    {
        for (int col = 0; col < MAX_COLUMNS; col++) //Loops through cols
        {
            cout <<  _table[row][col] << " "; //Prints out val
        }
        cout << endl;
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos)
{
    int inc = 0; //Sets an increment val to 0.

    while (s[inc] != NULL) //Loops for entire array.
    {
        cout << s[inc];
        if (inc == _pos) //Checks for position to mark.
        {
            cout << "[" << s[inc] << "]";
        }
        inc++;
    }

    cout << "  pos: " << _pos << endl;
}