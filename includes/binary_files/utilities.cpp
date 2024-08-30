#include "utilities.h"

using namespace std;

bool file_exists(const char filename[])
{
    const bool debug = false;
    fstream file;
    file.open (filename, fstream::in |fstream::binary );

    if (file.fail()) //Checks if file opens or not...
    {
        if (debug) 
        {
            cout << "DEBUG: file doesn't exist!, " << filename << endl;
        }
        
        return false;
    }

    file.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[])
{
    const bool debug = false;

    if (file_exists(filename) == false) //if file does not exist
    {
        f.open(filename, fstream::out| fstream::binary);

        if (f.fail()) //Checks if file fails to open
        {
            cout << "file open (RW) failed: " <<filename << endl;
            throw("fileRW has failed!");
        }
        else
        {
            if (debug)
            {
                cout << "DEBUG: file has been opened successfully: " << filename << endl;
            }
        }
    }
    else
    {
        f.open (filename, fstream::in| fstream::out| fstream::binary); //opens file

        if (f.fail())
        {
            cout << "fileRW failed... " << filename <<endl;
            throw("file did not open...");
        }
    }
}

void open_fileW(fstream& f, const char filename[])
{
    f.open (filename, fstream::out| fstream::binary);
    
    if (f.fail())
    {
        cout<<"file open failed: "<<filename<<endl;
        throw("file did not open...");
    }
}

vector<string> read_info(fstream& f, const char filename[], long recno, int filesize)
{
    const bool debug = false;
    long position = recno * filesize;
    f.seekg(position, ios_base::beg);

    if (!file_exists(filename)) //Checks if file exists
    {

        f.open(filename, fstream::out|fstream::binary);

        if (f.fail()) //Checks if file opened or not
        {
            cout << "file open failed, " << filename <<endl;
            throw("file RW failed ");
        }
        else
        {
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else
    {
        f.open (filename, fstream::in | fstream::out| fstream::binary);

        if (f.fail())
        {
            cout<<"file open failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

    FileRecord record2;
    long bytes = record2.read(f, recno);

    f.close();
    
    return record2.get_record();
}