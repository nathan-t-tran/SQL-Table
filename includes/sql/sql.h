#ifndef SQL_H
#define SQL_H
#include <iostream>
#include <iomanip>
#include "../../includes/table/table.h"
#include "../../includes/parser/parser.h"

using namespace std;

class SQL{
public:

    SQL(){}
    Table command(string input_string)
    {
        //initializes elements needed

        //cout << "INPUT(SQL::COMMAND): " << input << endl;

        strcpy(input_array, input_string.c_str());
        Parser parse_me; //get parser
        parse_me = Parser(input_array);

        //cout << "INPUT ARRAY: " << input_array << endl;

        //sets commands to parse_me
        mmap_ss commands; //mmap to hold user commands
        commands = parse_me.parse_tree(); 
        
        //cout<< "COMMANDS: " << commands <<endl;
        //fields:[last]
        //conditions =  [=][Johnson]

        string command_entered = commands["command"][0]; //WHAT THE USER WANTS TO DO
        if (command_entered == "") //invalid command
        {
            cout << "INVALID COMMAND" << endl;
            return Table();
        }

        if(command_entered == insert)
        { 
            //intializes objects
            //cout << "Grabbing names" << endl;
            string table_name = commands["table_name"][0];

            vector<string> input_command = commands["values"];
            Table copy_table(table_name);

            //cout name
            //cout << "TABLE_NAME: " << table_name << endl;

            //inserts _table
            //cout << "Inserting into GIVEN TABLE" << endl;
            copy_table.insert_into(input_command);
            //cout << "INPUT: " << input << endl;
            //cout << "COPY_TABLE: " << copy_table << endl;
            return copy_table;
        }
        else if (command_entered == make||command_entered == create)
        {  
            //intializes objects
            //cout << "MAKING NEW TABLE" << endl;
            string table_name = commands["table_name"][0];
            vector<string> new_fields = commands["col"];
            Table new_table(table_name, new_fields);
            
            return new_table;
        } 
        else if (command_entered == select)
        { 
            //intializes objects
            //cout << "SELECTING FROM " << endl;

            string table_names = commands["table_name"][0];
            select_command_table = Table(table_names); 
            
            //cout << "selects: " << selects << endl;

            if(!commands.contains("where"))
            { 
                vector<string> fields = commands["fields"];

                //cout << "FIELDS (SQL 122): " << fields << endl;
                //cout << "selects: " << selects.select(fields) << endl;
                Table return_selects = select_command_table.select(fields);
                //cout << "RETURNED_SELECTS: " << return_selects << endl;
                return return_selects;
            }
            else
            {
                //gets the conditions
                vector<string> conditions = commands["condition"];

                //cout << "CONDITIONS: " << commands << endl;

                //gets the fields
                vector<string> fields = commands["fields"];

                //cout << "FIELDS: " << fields << endl;
                Table return_selects = select_command_table.select(fields, conditions);
                //cout << "RETURNED_SELECTS: " << return_selects << endl;
                return return_selects;
            }
            
            if(commands["fields"][0] == "*")
            { 
                if(!commands.contains("where"))
                { 
                    //returns select_all
                    Table return_selects = select_command_table.select_all();
                    //cout << "RETURNED_SELECTS: " << return_selects << endl;
                    return return_selects;
                }
                else
                {
                    //cout << "I AM HAPPENING IN SQL 99" << endl;

                    //gets fields in field name
                    //gets necessary files and strings
                    string text_name = table_names + "_fields_" + ".txt"; 
                    ifstream if_file(text_name);
                    vector<string> name_field;

                    //while file is open
                    while(!if_file.eof())
                    { 
                        string field;
                        getline(if_file, field);

                        //cout << "FIELD for if_file: " << field << endl;

                        name_field.push_back(field);
                    }

                    //cout << "I AM AT GET_FIELDS: I AM GETTING FIELDN: " << fieldn << endl;
                    if_file.close(); //closes files

                    //cout << "NAME_FIELD: " << name_field << endl;

                    //gets the conditions
                    vector<string> conditions = commands["condition"];

                    //cout << "CONDITIONS: " << conditions << endl;
                    //cout << "FIELDN (SQL 111): " << fieldn << endl;
                    Table return_selects = select_command_table.select(name_field, conditions);
                    //cout << "RETURNED_SELECTS: " << return_selects << endl;
                    return return_selects;
                }
            }
        }
    }
    vector<long> get_record_num()
    {
        record_num = select_command_table.get_record_num();
        return record_num;
    }
    vector<string> get_tables()
    {
        return tables;
    }

private:    
    vector<long> record_num;
    vector<string> tables;
    Table select_command_table; 
    char input_array[200];
    const string make = "make";
    const string insert = "insert";
    const string create = "create";
    const string select = "select";
};



#endif