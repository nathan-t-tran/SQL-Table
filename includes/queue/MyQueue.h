#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <iostream>
#include <iomanip>
#include "../../includes/linked_list_functions/linkedlistfunctionsTemplated.h"
#include "../../includes/node/node.h"

using namespace std;

template <typename T>
class Queue
{
public:
    class Iterator{
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator(){_ptr = NULL;}                          //default ctor
        Iterator(node<T>* p)                            //Point Iterator to where p is pointing to
        {
            _ptr = p;
        }                                            
        T operator *()    //dereference operator
        {
            return _ptr->_item;
        }
        bool is_null()            //true if _ptr is NULL
        {
            return (_ptr == nullptr);
        }
        friend bool operator !=(const Iterator& left, const Iterator& right)  //true if left != right                     
        {
            return (left._ptr != right._ptr);
        }

        friend bool operator ==(const Iterator& left, const Iterator& right)  //true if left == right                      
        {
            return (left._ptr == right._ptr);
        }


        Iterator& operator++()  //member operator:++it or ++it = new_value
        {                                            
            _ptr = _ptr->_next;
            return *this;
        }

        friend Iterator operator++(Iterator& it, int unused)         //friend operator: it++
        {
            Iterator temp(it._ptr);
            it._ptr = it._ptr->_next;
            return temp;
        }

    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Queue()
    {
        _front = nullptr; //Sets the beginning of the queue to null.
        _rear = _front; //Sets the end of the queue to the front ptr since empty.
        _size = 0;
    }

    Queue(const Queue<T>& copyMe)
    {
        _front = nullptr; //Creates an empty head.
        _front = _copy_list(copyMe._front); //Copies data from one head to another
        _rear = copyMe._rear; //Sets rear node to copyMe rear
        _size = copyMe._size; //Gets new size.
    }

    ~Queue()
    {
        _clear_list(_front); //Empties list.
    }

    Queue& operator=(const Queue<T>& RHS)
    {
        _front = nullptr; //Creates an empty head.
        _front = _copy_list(RHS._front); //Copies data over to another head
        _rear = RHS._rear; //Sets rear
        _size = RHS._size; //Sets size
        return *this;
    }

    bool empty()
    {
        return (_front == nullptr);
    }
    T front()
    {   
        assert(_front != nullptr);
        return _front->_item;
    }
    T back()
    {
        assert(_rear != nullptr);
        return _rear->_item;
    }
    void push(T item)
    {
        _rear = _insert_after(_front, _rear, item);
    }
    T pop()
    {
        assert(_front != nullptr);
        T item_to_return = _delete_node(_front, _front); //Gets the item from the list.
        return item_to_return;
    }
    Iterator begin() const                                     //Iterator to the head node
    {
        return Iterator(_front);
    }
    Iterator end() const                                       //Iterator to NULL
    {
        return Iterator();
    }
    void print_pointers();
    int size() const { return _size; }
    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& printMe);
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

template<typename TT>
ostream& operator << (ostream& outs, const Queue<TT>& printMe)
{
    cout << "Queue:";
    _print_list (printMe._front);
    return outs;
}

#endif