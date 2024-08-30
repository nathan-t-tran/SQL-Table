#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <iomanip>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include "typedefs.h"
#include "../../includes/bplustree/multimap.h"
#include "../../includes/bplustree/map.h"
#include "../../includes/binary_files/file_record.h"
#include "../../includes/queue/MyQueue.h"
#include "../../includes/token/token_base.h"
#include "../../includes/token/operator.h"
#include "../../includes/token/leftparen.h"
#include "../../includes/token/rightparen.h"
#include "../../includes/token/relational.h"
#include "../../includes/token/logical.h"
#include "../../includes/token/token_str.h"
#include "../../includes/shunting_yard/shunting_yard.h"
#include "../../includes/token/resultset.h"

using namespace std;

class Table{
public:
    Table()
    {   
        last_record = 0;
        names = "";
        isEmpty = true;
    }

    Table(const string& name, const vector<string> field_names)
    {
        names = name;
        _field_names_vector = field_names;

        for(int i=0; i<_field_names_vector.size(); i++)
        {  
            field_map.insert(_field_names_vector[i], i);
            indi.push_back(MMap<string, long>()); 
        }

        //cout << "15" << endl;

        isEmpty = true;
        last_record = 0;

        string file_name = name + ".bin";

        //cout << "20" << endl;
        
        open_fileW(file_stream, file_name.c_str());

        //cout << "25" << endl;

        string txt_file_name = names + "_fields_" +".txt"; 
        ofstream of_file(txt_file_name); 

        //cout << "30" << endl;

        if (!of_file.is_open()) 
        { 
            cout << "DEBUG: file could not be opened, exiting ctor" << endl;
            return;
        }

        //cout << "35" << endl;

        for(int i=0; i < _field_names_vector.size(); i++) 
        {
            of_file << _field_names_vector[i];
            if (i < _field_names_vector.size() - 1)
            {
                of_file << endl;
            } 
        }

        //cout << "40" << endl;

        file_stream.close();
        of_file.close();    

        //cout << "50" << endl;

    } 
    
    Table(const string& name)
    {
        names = name;
        last_record = 0;
        
        string txt_file_name = names + "_fields_" + ".txt";
        
        ifstream if_file(txt_file_name);
        vector<string> fieldname;
        while(!if_file.eof())
        { 
            string field;
            getline(if_file, field);
            fieldname.push_back(field);
        }

        if_file.close();
        _field_names_vector = fieldname;
        
        for(int i=0; i<_field_names_vector.size(); i++)
        {  
            field_map.insert(_field_names_vector[i], i);
            indi.push_back(MMap<string, long>()); 
        }

        FileRecord record2;

        open_fileRW(file_stream, (names+".bin").c_str());
        
        //cout << "TABLE NAME: " << names << endl;
        long check_bytes = record2.read(file_stream, last_record);
        //cout << "BANANA" << endl;
        
        while(check_bytes > 0)
        { 
            last_record++;
            check_bytes = record2.read(file_stream, last_record);
        }
        //cout<<"APPLE"<<endl;
        file_stream.close();
        last_record--;

        vector<string> vector_string;
        //cout<<"ORANAGE"<<endl;
        //cout << last_record << endl;
        for ( int i=0; i <= last_record; i++) 
        {
            //cout<<"MANGO"<<endl;
            vector_string = read_info(file_stream, (names+".bin").c_str(), i, 10100); //reads info
            //cout<<"VECTOR: "<<vector_str<<endl;
            file_stream.close(); //closes file

            for (int j=0; j < vector_string.size(); j++) 
            {
                indi[j].insert(vector_string[j], i);
            }
            //cout << "APES" << endl;
        }
    }


    void insert_into(const vectorstr& v)
    {
        open_fileRW(file_stream, (names+".bin").c_str()); //Opens files

        //cout << names << endl;

        //Initializes vars
        FileRecord file_record(v);
        int record_number;
        record_number = file_record.write(file_stream); 
        last_record = record_number; 

        //cout << "INSERT HAPPEN" << endl;

        //cout << "v.size(): " << v.size() << endl;

        //cout << v << endl;

        for(int i = 0; i < v.size(); i++)
        {
            //cout << "i am currently happening" << endl;

            int index = field_map[_field_names_vector[i]];

            //cout << "v[i]: " << v[i] << ", index: " << index << endl;

            indi[index].insert(v[i], record_number);

            //cout << "I AM HAPPENING: " << i << endl;
        }

        //cout << "INSERT HAPPENED" << endl;

        isEmpty = false;
        file_stream.close();
    }

    Table select(vector<string> name, const string& field, const string& relational_string, const string& value) 
    {
        //initializes variables
        int index_vector = field_map[field];
        vector<long> copy_vector_vector; 

        if(relational_string == "=")
        {
            copy_vector_vector += indi[index_vector][value];
            record_num = copy_vector_vector;
            
        }
        else if(relational_string == ">")
        {
            MMap<string, long>::Iterator iterator = indi[index_vector].upper_bound(value);
            MMap<string, long>::Iterator iterator_end = indi[index_vector].end();

            for(;iterator!=iterator_end; iterator++)
            {
                copy_vector_vector += (*iterator).value_list;
            }
            record_num = copy_vector_vector;
            
        }
        else if(relational_string == "<")
        {
            MMap<string, long>::Iterator iterator = indi[index_vector].lower_bound(value);
            MMap<string, long>::Iterator start_iterator = indi[index_vector].begin();
            
            for(;(*start_iterator).key<(*iterator).key; start_iterator++)
            {
                copy_vector_vector += (*start_iterator).value_list;
            }
            record_num = copy_vector_vector;
        }
        else if (relational_string == ">=")
        {
            MMap<string, long>::Iterator iterator = indi[index_vector].lower_bound(value);
            MMap<string, long>::Iterator iterator_end = indi[index_vector].end();

            for(;iterator!=iterator_end; iterator++)
            {
                copy_vector_vector += (*iterator).value_list;
            }
            record_num = copy_vector_vector;
        }
        else if (relational_string == "<=")
        {
            MMap<string, long>::Iterator iterator = indi[index_vector].lower_bound(value);
            MMap<string, long>::Iterator start_iterator = indi[index_vector].begin();
            
            for(;(*start_iterator).key<=(*iterator).key; start_iterator++
            ){
                copy_vector_vector += (*start_iterator).value_list;
            }
            
            record_num = copy_vector_vector;
        }
        
        serial++;
        string new_table = "table_selected_"+to_string(serial);
        Table temp(new_table, name);

        if(copy_vector_vector.size()!=0)
        {
            for(int i=0; i<copy_vector_vector.size(); i++)
            {
                vector<string> insert = read_info(file_stream, (names+".bin").c_str(), copy_vector_vector[i], 10100);
                vector<string> file; 

                for(int j = 0; j < name.size(); j++)
                {
                    //inserts file into insert
                    file.push_back(insert[field_map[name[j]]]);
                }
            
                temp.insert_into(file);
            }
            file_stream.close();
        }
        else
        {
            temp.isEmpty = true;
        }
        return temp; 
    }

    Table select(vector<string>& fields)
    {
        serial++;

        string new_table = "temp_table_"+to_string(serial);
        Table table_temp(new_table, fields);

        //cout << "FIELDS (TABLE::SELECT): " << fields << endl;

        for(int i=0; i < last_record + 1; i++)
        {
            record_num.push_back(i);

            vector<string> insert_vector = read_info(file_stream, (names+".bin").c_str(), i, 10100);

            //cout << "INSERT_VECTOR: " << insert_vector << endl;

            vector<string> user_field; 

            //cout << "FIELDS.SIZE(): " << fields.size() << endl;

            for(int j = 0; j < fields.size(); j++)
            {
                user_field.push_back(insert_vector[field_map[fields[j]]]);

                //cout << "USER_FIELD: " << user_field << endl;
            }
            table_temp.insert_into(user_field);

            //cout << "TABLE_TEMP: " << table_temp << endl;
        }
        return table_temp;
    }
    
    Table select(const vector<string>& fields, Queue<Token_Base*>& postfix)
    {
        serial++; //increments serial

        //initializes vars and objects...
        string new_table = "temp_table_" + to_string(serial);
        Table table_temp(new_table, fields);

        //cout << "FIELDS: " << fields << endl;
        //cout << "POSTFIX(TABLE): " << postfix << endl;

        Queue<Token_Base*> postfix_process = postfix; 

        //cout << "CHECKING IF POST WORKED: " << postfix_process << endl;

        Stack<Token_Base*> Stack;
        Token_Base* popped;

        //processes the postfix
        while(!postfix_process.empty())
        { 
            popped = postfix_process.pop();
            //cout << "POPPED: " << *popped << endl;
            //cout << "popped type: " << popped->TypeOf() << endl;

            if(popped->TypeOf()== 8) //if popped is a string
            {
                Stack.push(popped);
                //cout << "STACK: " << Stack << endl;
                
            } 
            if (popped->TypeOf()== 2) //if popped is an operator...
            {
                vector<long> records;
                //processes values and fields
                Token_Base* pop2 = Stack.pop(); 
                //cout << "POP2: " << pop2->TkStr() << endl;
                Token_Base* pop1 = Stack.pop();
                //cout << "POP1: " << pop1->TkStr() << endl;

                //cout << "popped type: " << popped->TypeOf() << endl;

                //cout << "PREC: " << static_cast<Operator*>(popped)->get_prec() << endl;

                if (static_cast<Operator*>(popped)->get_prec() == 5) //Checks for relational_string tokens
                {
                    //cout << "I AM EVALUATING IN relational_string" << endl;

                    string pop_val = popped->TkStr();
                    string values = static_cast<TokenStr*>(pop2)->str();

                    int vec_index = field_map[static_cast<TokenStr*>(pop1)->str()];

                    if (pop_val == "=")
                    {
                        vector<long> copy_vector = indi[vec_index][values];
                        records += copy_vector;
                    }
                    else if (pop_val == "<")
                    {   
                        vector<long> copy_vector;  

                        MMap<string, long>::Iterator it = indi[vec_index].lower_bound(values);
                        MMap<string, long>::Iterator start_iterator = indi[vec_index].begin();
                        

                        for(;(*start_iterator).key<(*it).key; start_iterator++)
                        {
                            copy_vector += (*start_iterator).value_list;
                        }
                    
                        records += copy_vector;
                    }
                    else if (pop_val == ">")
                    {   
                        vector<long> copy_vector;  

                        MMap<string, long>::Iterator it = indi[vec_index].upper_bound(values);
                        MMap<string, long>::Iterator iterator_end = indi[vec_index].end();
                        
                        for(;it!=iterator_end; it++)
                        {
                            copy_vector += (*it).value_list;
                        }
                        
                        records += copy_vector;
                    }
                    else if (pop_val == "<=")
                    {
                        vector<long> copy_vector; 
                        
                        MMap<string,long>::Iterator it = indi[vec_index].lower_bound(values);
                        MMap<string,long>::Iterator start_iterator = indi[vec_index].begin();

                        for(;(*start_iterator).key<(*it).key; start_iterator++)
                        {
                            copy_vector += (*start_iterator).value_list;
                        }

                        if((*start_iterator).key==values)
                        {
                            copy_vector += (*start_iterator).value_list;
                        }
                        
                        records += copy_vector;
                    }
                    else if( pop_val == ">=")
                    {
                        vector<long> copy_vector;

                        MMap<string,long>::Iterator it = indi[vec_index].lower_bound(values);
                        MMap<string,long>::Iterator iterator_end = indi[vec_index].end();

                        for(;it!=iterator_end; it++)
                        {
                            copy_vector += (*it).value_list;
                        }
                        
                        records += copy_vector;
                    }
                }
                else if (static_cast<Operator*>(popped)->get_prec() == 4 || static_cast<Operator*>(popped)->get_prec() == 3) //evaluates logical tokens
                {
                    //cout << "I AM EVALUATING IN LOGICAL" << endl;

                    vector<long> vector_1 = static_cast<ResultSet*>(pop1)->get_recnos();
                    vector<long> vector_2 = static_cast<ResultSet*>(pop2)->get_recnos();

                    sort(vector_1.begin(), vector_1.end());
                    sort(vector_2.begin(), vector_2.end());

                    if(popped->TkStr() == "and") //AND case
                    { 

                        set_intersection(vector_1.begin(), vector_1.end(), vector_2.begin(), vector_2.end(), back_inserter(records));
                                    
                    }
                    else //OR case
                    {               
                        set_union(vector_1.begin(), vector_1.end(), vector_2.begin(), vector_2.end(), back_inserter(records));
                    }
                }

                vector<long> recs = records;

                //cout << "RECS: " << recs << endl;

                Stack.push(new ResultSet(recs));
            }
        }

        //cout << "STACK: " << Stack << endl;
        Token_Base* end = Stack.pop();

        //cout << "TOKEN_END: " << end->TkStr() << endl;

        //gets final set after processing
        vector<long> final_set = static_cast<ResultSet*>(end)->get_recnos();

        //cout << "FINAL_SET: " << final_set << endl;

        record_num = final_set;

        //cout << "RECORD_NUM: " << record_num << endl;

        if(final_set.size()!=0)
        {
            for(int i=0; i<final_set.size(); i++)
            {  
                //reads info
                vector<string> insert_vector = read_info(file_stream, (names+".bin").c_str(), final_set[i], 10100);

                //cout << "INS: " << ins << endl;

                //Gets user fields
                vector<string> user_fields;

                for(int j = 0; j < fields.size(); j++)
                {
                    user_fields.push_back(insert_vector[field_map[fields[j]]]);
                }
                table_temp.insert_into(user_fields);

                //cout << "TABLE_TEMP(TABLE): " << table_temp << endl;

            }

            //Closes file
            file_stream.close(); 
        }
        else
        {
            table_temp.isEmpty = true;
        }
        return table_temp;
    }
    
    Table select(const vector<string>& fields, vector<string>& string)
    {
        //For Shunting Yard...
        ShuntingYard shunting_yard(string); 
        shunting_yard.infix();   
        Queue<Token_Base*> postfix_process = shunting_yard.postfix(); 

        //cout << "POST: " << postfix_process << endl;

        return select(fields, postfix_process);    
    } 

    Table select_all() 
    {
        serial++; 

        //Gets new table and gets a temporary table for field names
        string new_table = "temp_table_"+to_string(serial);
        Table table_temp(new_table, _field_names_vector);

        //cout << "VECTOR_FN: " << _field_names_vector << endl;
        //cout << "table_temp: " << table_temp << endl;
        
        for(int i = 0; i < last_record + 1; i++)
        { 
            //record_num.push_back(i);

            //cout << "RECORD_NUM: " << record_num << endl;

            //gets info into insert_vector
            vector<string> insert_vector = read_info(file_stream, (names+".bin").c_str(), i, 10100);

            table_temp.insert_into(insert_vector);
        }
        return table_temp;
    }

    
    vector<long> get_record_num()
    {
        return record_num;
    }

    void set_fields(vector<string> in_field_name)
    {
        _field_names_vector = in_field_name;
    }

    vectorstr get_fields()
    { 
        return _field_names_vector;
    }

    friend ostream& operator << (ostream& outs, const Table& t)
    {
        if(t.isEmpty == false) //Checks if the table is empty for printing
        {
            outs << "Table Name: " << t.names <<", Records: " << t.last_record+1 << " Serial: " << t.serial << endl; 
        }
        else
        {
            outs << "Table Name: " << t.names << ", Records: 0 " << " Serial: " << t.serial << endl; 
        }
        outs << setw(25) << "Record";

        //Columns
        for (int i=0; i<t._field_names_vector.size(); i++)
        {
            outs<<setw(25)<<t._field_names_vector[i];
        }
        outs<<endl;

        if(t.isEmpty != false)
        {
            fstream temp_f((t.names+".bin").c_str());
            
            //Rows
            for(int i=0; i<=t.last_record; i++)
            { 
                outs<<setw(25)<<i;
                FileRecord temp_r;
                int recno = temp_r.read(temp_f, i);
                outs << setw(25) << temp_r <<endl;
            
            }
            temp_f.close();
        }
        return outs;
    }
    
private:
    vector<string> _field_names_vector;
    vector<long> record_num;
    vector<MMap<string, long>> indi;
    fstream file_stream;
    string names;
    Map<string, long> field_map;
    long last_record;
    bool isEmpty;
    long serial = 0;
};

#endif
