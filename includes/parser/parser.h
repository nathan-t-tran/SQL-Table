#ifndef PARSER_H
#define PARSER_H

#include "../../includes/table/table.h"
#include "../../includes/tokenizer/stokenize.h"
#include "sql_parser_functions.h"
#include <algorithm>
#include <vector>
#include <string>
#include "typedef.h"

using namespace std;

class Parser{
public:
    Parser();
    Parser(char* s);
    void make_table();
    void set_string(char* s); 
    void get_column(); 
    mmap_ss parse_tree(); 
    bool get_parse_tree(); 
    void build_keyword_list(); 
    bool failed();
private:
    MMap<string, string> _parser_Tree;
    Map<string, int> _map_of_keys;
    vector<string> _input_q;
    static int _table[20][20];
    bool _check_Fail;
    char _buffer[200];
    enum keys {};
};


#endif