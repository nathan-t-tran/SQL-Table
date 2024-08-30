#include "parser.h"

int Parser::_table[20][20];

Parser::Parser(){}

Parser::Parser(char* s)
{
    //puts s into _buffer...
    strcpy(_buffer, s);

    //Creates essentials for parser...
    make_table();
    build_keyword_list();
    set_string(_buffer);

    //cout << "BUFFER: " << _buffer << endl;
}

void Parser::make_table()
{
    init_ptable(_table);

    //Marks success states
    mark_psuccess(_table, 4);
    mark_psuccess(_table, 6); 
    mark_psuccess(_table, 11); 
    mark_psuccess(_table, 16);

    //MAKES/CREATES
    mark_pcell(0, _table, 5, 7); 
    mark_pcell(0, _table, 11, 7); 
    mark_pcell(7, _table, 6, 8); 
    mark_pcell(8, _table, 15, 9); 
    mark_pcell(9, _table, 7, 10); 
    mark_pcell(10, _table, 15, 11); 
    mark_pcell(11, _table, 15, 11); 

    //INSERTS
    mark_pcell(0, _table, 8, 12); 
    mark_pcell(12, _table, 9, 13); 
    mark_pcell(13, _table, 15, 14); 
    mark_pcell(14, _table, 10, 15); 
    mark_pcell(15, _table, 15, 16); 
    mark_pcell(16, _table, 15, 16); 

    //SELECTS
    mark_pcell(0, _table, 1, 1); 
    mark_pcell(1, _table, 15, 2); 
    mark_pcell(2, _table, 15, 2); 
    mark_pcell(1, _table, 4, 2); 
    mark_pcell(2, _table, 2, 3); 
    mark_pcell(3, _table, 15, 4); 
    mark_pcell(4, _table, 3, 5); 
    mark_pcell(5, _table, 15, 6); 
    mark_pcell(6, _table, 15, 6); 

    //cout << "TABLE: ";
    //print_ptable(_table);
}

void Parser::set_string(char* s)
{
    _parser_Tree.clear();
    _input_q.clear();

    strcpy(_buffer, s);
    STokenizer string_tokenizer(_buffer);
    //wcout<<"BUFFER(SET_STRING): "<<_buffer<<endl;
    
    while(!string_tokenizer.done())
    {
        //Runs string through stokenizer
        Token string_Tokenizer_token;
        string_tokenizer >> string_Tokenizer_token;
        //cout<<"PARSER TOKEN: "<< token <<endl;
        //cout<<"TOKEN TYPE: " << token.type()<<endl;


        //Checks for relational strings
        if(string_Tokenizer_token.token_str() == "*"||string_Tokenizer_token.token_str()== "("||string_Tokenizer_token.token_str() == ")"||string_Tokenizer_token.token_str() == "="|| string_Tokenizer_token.token_str() == ">")
        {
            _input_q.push_back(string_Tokenizer_token.token_str());
            continue;
        }

        //where bug occurs: it processes "Billy" but does not push it...
        //"Billy" is type 5 for some reason...
        

        if(string_Tokenizer_token.type() == 3||string_Tokenizer_token.type() == 5||string_Tokenizer_token.type() == -2||string_Tokenizer_token.type() == -1)
        {
            continue;
        }
        
        if(string_Tokenizer_token.type() == 9)
        {
            _input_q.push_back((string_Tokenizer_token.token_str()).substr(1, string_Tokenizer_token.token_str().size()-2));
        }
        else
        {
            _input_q.push_back(string_Tokenizer_token.token_str());
            // cout<<"WEIRD CASE "<<token.token_str()<<endl;
        }
        //cout << "_input_q: " << _input_q << endl;
    }

    if(get_parse_tree())
    {
        _check_Fail = false;
    }
    else
    {
        _check_Fail = true;
    }
   
} 
void Parser::get_column(){}

mmap_ss Parser::parse_tree()
{
    return _parser_Tree;
} 

bool Parser::get_parse_tree()
{
    int column = 0;
    int check_State = 0;
    string check_Me_token;
    //cout<<_input_q<<endl;
    for(int i=0; i<_input_q.size(); i++)
    {
        check_Me_token = _input_q[i];
        string check_Me = check_Me_token;

        transform(check_Me.begin(), check_Me.end(), check_Me.begin(), [](unsigned char c){return tolower(c);});

        if(_map_of_keys.contains(check_Me))
        {
            column = _map_of_keys[check_Me];
        }
        else
        {
            column = 15; 
        }
        check_State = _table[check_State][column];
        
        //If fail state, exit
        if(check_State == -1)
        {
            break;
        }

        //Checks for the different states
        if(check_State == 1||check_State == 7||check_State == 12)
        {   
            _parser_Tree["command"] += check_Me;
            
        } 
        else if(check_State == 2)
        {  
            _parser_Tree["fields"] += check_Me_token;
        } 
        else if(check_State==11)
        {
            _parser_Tree["col"] += check_Me_token;
        }
        else if(check_State==16)
        {
            _parser_Tree["values"] += check_Me_token;
        }
        else if(check_State==4||check_State==9||check_State==14)
        {  
            _parser_Tree["table_name"] += check_Me_token;
        } 
        else if (check_State == 5)
        {   
            _parser_Tree["where"].push_back("yes");
        } 
        else if (check_State == 6)
        {
            _parser_Tree["condition"] += check_Me_token;
        }

    }
    
    if(check_State == -1) //Fail check_State
    { 
        _parser_Tree.clear();
        return false;
    }
    return true;

}
void Parser::build_keyword_list()
{
    //CREATES COMMANDS FOR COMMAND LIST
    _map_of_keys.insert("select", 1); 
    _map_of_keys.insert("from", 2);
    _map_of_keys.insert("where", 3);
    _map_of_keys.insert("*", 4);
    _map_of_keys.insert("make", 5);  
    _map_of_keys.insert("create", 11); 
    _map_of_keys.insert("table", 6);
    _map_of_keys.insert("fields", 7);
    _map_of_keys.insert("insert", 8); 
    _map_of_keys.insert("into", 9);
    _map_of_keys.insert("values", 10);
} 

bool Parser::failed()
{
    return _check_Fail;
}