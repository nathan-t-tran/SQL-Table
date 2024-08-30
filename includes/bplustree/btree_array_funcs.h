#ifndef BTREE_ARRAY_FUNCS_H
#define BTREE_ARRAY_FUNCS_H
#include <cassert>
#include <iostream>
using namespace std;

template <class T>
T maximal(const T& a, const T& b) //return the larger of the two items
{
    if(a >= b)
    {
        return a;
    } 
    else 
    {
        return b;
    }
}                      

template <class T>
void swapper(T& a, T& b) //swap the two items
{
    T temp = a;
    a = b;
    b = temp;
}  

template <class T>
int index_of_maximal(T data[ ], int n)                 //return index of the largest item in data
{
    const bool debug = false;
    int index;
    T current_max = data[0];

    for (int i = 1; i < n; i++)
    {
        if (data[i] > current_max)
        {
            if (debug)
            {
                cout << "WHEN DATA IS LARGER" << endl;
                cout << "data[index] = " << data[i] << ", current_max = " << current_max << endl;
                cout << "index: " << index << endl;
            }
            current_max = data[i];
            index = i;
        }
        if (debug)
        {
            cout << "Index: " << index << endl;
        }
    }
    return index;

}               

template <class T>
void ordered_insert(T data[ ], int& n, T entry) //insert entry into the sorted array data with length n
{
    int i=0;
    while (i<n && data[i]<entry){i++;}
    for(int j = n; j > i; j--)
    {
        data[j] = data[j-1];
    }
    data[i] = entry;
    n++;
}        
                                                        

template <class T>
int first_ge(const T data[ ], int n, const T& entry)   //return the first element in data that is not less than entry
{
    int i = 0;
    for (i; i < n; i++)
    {
        if (data[i] >= entry)
        {
            return i; //returns index of first element that is not less than entry.
        }
    }
    return i;
}


template <class T>
void attach_item(T data[ ], int& n, const T& entry)    //append entry to the right of data
{
    n++; //Increases size by 1
    data[n-1] = entry; //sets far right of data to entry.
}

template <class T>
void insert_item(T data[ ], int i, int& n, T entry)    //insert entry at index i in data
{
    const bool debug = false;

    for (int j = n - 1; j >= i; j--)
    {
        if (debug)
        {
            cout << "DEBUG: data[j+1] = " << data[j+1] << endl; 
        }
        data[j + 1] = data[j];
    }
    data[i] = entry;
    n++;
}

template <class T>
void detach_item(T data[ ], int& n, T& entry)          //remove the last element in data and place it in entry
{
    n--;
    entry = data[n];
    data[n] = NULL; 
}

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){
    if(i == n-1) //Checks if index is last entry
    {
        detach_item(data, n, entry);
        return;
    }

    entry = data[i];

    for(i; i < n - 1; i++) //For shifting
    { 
        T* shift = data+i; //Gets walkers
        T* copy = shift+1;
        *shift = *copy;
    }
    n--;
}   

template <class T>
void delete_ptr(T* data[ ], int i, int& n) { 
    assert(i < n && i >= 0 && "invalid index");

    for( i; i < n; i++) 
    { 
        data[i] = data[i+1];
    }

    if (n > 0) {n--;}
}   

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)   //append data2 to the right of data1
{
    const bool debug = false;
    for (int i = 0; i < n2; i++)
    {
        T item_to_add = data2[i];

        if (debug)
        {
            cout << "DEBUG: item_to_add: " << item_to_add << endl;
        }

        insert_item(data1, n1, n1, item_to_add);
    }
}   
template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)   //move n/2 elements from the right of data1 and move to data2
{
    int index = 0;

    if(n1 % 2 == 1)
    {
        index = n1/2 + 1;
    } 
    else 
    {
        index = n1/2;
    }

    for(index; index < n1; index++)
    {
        attach_item(data2, n2, data1[index]);
    }

    if(n1 % 2 == 1)
    {
        index = n1/2 + 1;
    } 
    else 
    {
        index = n1/2;
    }
    n1 = index;
}

template <class T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size)
{
    for (int i = 0; i < src_size; i++)
    {
        dest[i] = src[i];
    }  
    dest_size = src_size;
}       

template <class T>
void print_array(const T data[], int n, int pos = -1)
{
    cout << "[";
    for (int i = 0; i < n; i++)
    {
        cout << data[i] << "|";
    }
    cout << "]";
}  

template <class T>
bool is_gt(const T data[], int n, const T& item
){
    for (int i = 0; i < n; i++)
    {
        if(data[i]>item)
        {
            return false;
        }
    }
    return true;
}     

template <class T>
bool is_le(const T data[], int n, const T& item)
{
    for (int i = 0; i < n; i++)
    {
        if(data[i] < item)
        {
            return false;
        }
    }
    return true;
}  

template <class T>
T* search(T arr[], int n, T key)
{
    for(int i = 0; i < n; i++)
    {
        if(arr[i]==key)
        {
            return &(arr[i]);
        }
    }
    return nullptr;
}

#endif
