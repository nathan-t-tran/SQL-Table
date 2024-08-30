#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include "file_record.h"

using namespace std;

bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]);
void open_fileW(fstream& f, const char filename[]);
vector<string> read_info(fstream& f, const char filename[], long recno, int filesize);

#endif