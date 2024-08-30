#ifndef VECTOR_UTILITIES_H
#define VECTOR_UTILITIES_H
#include <iostream>
#include <vector>
using namespace std;


template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list)
{
    for(int i=0; i<list.size(); i++){
        cout<<"["<<list[i]<<"]";
    }
    return outs;
} 

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme)
{
    list.push_back(addme);
    return list;
} 

template <typename T>
vector<T>& operator +=(vector<T>&list, const vector<T>& rhs)
{
    for(int i=0; i<rhs.size(); i++)
    {
        list += rhs[i];
    }
}





#endif