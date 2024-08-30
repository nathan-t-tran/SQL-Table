#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include "utilities.h"
#include <cassert>

using namespace std;

class FileRecord
{
    public:
        FileRecord()
        {
            num_of_fields = -1;
            _record[0][0] = '\0';
        }

        FileRecord(char str[])
        {
            strncpy(_record[0], str, MAX);
            num_of_fields++;
        }
        
        FileRecord(string s)
        {
            strncpy(_record[0], s.c_str(), MAX);
            num_of_fields++;
        }

        FileRecord(vector<string> s)
        {
            assert(s.size() <= MAX); //assert to check if size is less than or equal to max
            
            for(int i = 0; i < s.size(); i++)
            {
                assert(s[i].length() <= CMAX); //assert to check if length is less than or equal to cmax

                strncpy(_record[i], s[i].c_str(), MAX);
                num_of_fields++;
            }

            if(s.size()<MAX)
            {
                _record[s.size()][0]='\0';
            }
        }

        string operator[](int row)
        {
            string return_me = "";
            for (int i = 0; i < CMAX; i++)
            {
                return_me += _record[row][i];
            }
            return return_me;
        }

        vector<string> get_record() 
        {
            if (num_of_fields == -1) //Checks for null
            {
                return vector<string>();
            }

            string row = ""; 
            vector<string> record; 
            char* walker; 
            
            for(int i=0; i < num_of_fields+1; i++) 
            {
                walker = _record[i]; 

                while (*walker)  //Loops until walker is null
                {
                    row += *walker; 
                    walker++;
                }
                record.push_back(row); //inserts row...
                row = ""; 
            }

            return record;
        }

        long write(fstream& outs)
        {
            outs.seekg(0, outs.end);
            long pos = outs.tellp();    

            for(int i=0; i<MAX; i++)
            {
                outs.write(_record[i], sizeof(_record[i]));
            }
            
            return (pos/sizeof(_record));  
        }             

        long read(fstream& ins, long recno) 
        {
            long pos= recno * sizeof(_record);
            ins.seekg(pos, ios_base::beg);

            for(int i = 0; i < MAX; i++)
            {
                ins.read(_record[i], sizeof(_record[i]));
                _record[i][ins.gcount()] = '\0';

                if(_record[i][0]=='\0')
                {
                    break;
                }
                else
                {
                    num_of_fields++;
                }
            }
            return ins.gcount();
        }   
        
        friend ostream& operator<<(ostream& outs, const FileRecord& r)
        {
            for (int i=0; i < r.num_of_fields + 1; i++)
            {
                outs << setw(25) << r._record[i];
            }
            return outs;
        }

        int field_num(){return num_of_fields;}

private:
    static const int MAX = 100;
    static const int CMAX = 100;
    int num_of_fields;
    char _record[MAX][CMAX+1]; //NULL char
};


#endif
