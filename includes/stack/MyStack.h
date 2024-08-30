#ifndef MYSTACK_H
#define MYSTACK_H

#include <iostream>
#include <iomanip>
#include "../../includes/linked_list_functions/linkedlistfunctionsTemplated.h"
#include "../../includes/node/node.h"

using namespace std;

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator(){_ptr = NULL;}            //default ctor
        Iterator(node<ITEM_TYPE>* p){_ptr = p;}      //Point Iterator to where
                                            //  p is pointing to
        ITEM_TYPE operator *(){return _ptr->_item;}    //dereference operator
        bool is_null(){return _ptr == NULL;}            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  //true if left != right
        {return (left._ptr != right._ptr);}

        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  //true if left == right
        {return (left._ptr == right._ptr);}


        Iterator& operator++()                        //member operator: ++it or ++it = new_value
        {
            _ptr = _ptr->_next;
            return *this;
        }

        friend Iterator operator++(Iterator& it,
                                   int unused){        //friend operator: it++
            assert(it._ptr!=NULL);
            Iterator temp(it._ptr);
            it._ptr = it._ptr->_next;
            return temp;
        }

    private:
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    };

    Stack() //Initializes node.
    {
        _top = nullptr;
        _size = 0;
    }

    Stack(const Stack<ITEM_TYPE>& copyMe) //Copy CTOR
    {
        delete _top;
        _size = copyMe._size; //Copies size.
        _top = _copy_list(copyMe._top); //Copies node list.
    }

    ~Stack() //DESTRUCTOR
    {
        _clear_list (_top);
    }

    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS) //Assignment OP 
    {
        delete _top;
        _size = RHS._size; //Copies size
        _top = _copy_list(RHS._top); //Copies node list.
        return *this;
    }

    ITEM_TYPE top() //Returns the item of top node.
    {
        assert(_top != nullptr);
        return _top->_item; //Returns top item.
    }

    bool empty(){return _top == nullptr;}

    void push(ITEM_TYPE item)
    {
        _insert_head (_top, item);
        _size++;
    }

    ITEM_TYPE pop()
    {
        assert (_top != nullptr);
        ITEM_TYPE item_removed = _delete_node (_top, _top);
        _size--;
        return item_removed;
    }
    template<typename TT>
    friend ostream& operator<<(ostream& outs,  const Stack<TT>& printMe);
                                       
    Iterator begin() const              //Iterator to the head node
    {
        return Iterator(_top);
    }

    Iterator end() const               //Iterator to NULL
    {
        return Iterator();
    }
    int size() const { return _size; }

private:
    node<ITEM_TYPE>* _top;
    int _size;
};

template<typename TT>
ostream& operator<<(ostream& outs, const Stack<TT>& printMe)
{
    cout << "Stack:";
    _print_list(printMe._top);
    return outs;
}

#endif