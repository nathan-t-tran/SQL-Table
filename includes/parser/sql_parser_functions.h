#ifndef SQL_PARSER_FUNCTIONS_H
#define SQL_PARSER_FUNCTIONS_H
#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;


//Fill all cells of the array with -1
void init_ptable(int _table[][20]);

//Mark this state (row) with a 1 (success)
void mark_psuccess(int _table[][20], int state);

//Mark this state (row) with a 0 (fail)
void mark_pfail(int _table[][20], int state);

//true if state is a success state
bool is_psuccess(int _table[][20], int state);

//Mark a range of cells in the array. 
void mark_pcells(int row, int _table[][20], int from, int to, int state);

//Mark columns represented by the string columns[] for this row
void mark_pcells(int row, int _table[][20], const char columns[], int state);

//Mark this row and column
void mark_pcell(int row, int table[][20], int column, int state);

//This can realistically be used on a small table
void print_ptable(int _table[][20]);

#endif