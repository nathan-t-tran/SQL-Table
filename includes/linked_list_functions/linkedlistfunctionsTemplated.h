#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H
#define LINKEDLISTFUNCTIONSTEMPLATED_H

#include <iostream>
#include <iomanip>
#include "../../includes/node/node.h"

using namespace std;

//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head)
{
    node<T>* walker = head;
    cout << "Head->";
    while (walker != nullptr)
    {
        cout << "<-[" << *walker->_item << "]->";
        walker = walker->_next;
    }
    cout << " |||" << endl;
}

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head)
{

}

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key)
{
    const bool debug = false;
    node<T>* head_walker = head; //Gets walker for head
    
    //For debugging purposes...
    if (debug)
    {
        cout << "DEBUG: Before search: " << endl;
        cout << "head_walker: " << head_walker->_item << ", key: " << key << endl;
    }

    //Loops for the entire list
    while (head_walker != nullptr)
    {
        if (head_walker->_item == key) //Checks if the item of the walker matches the key
        {
            return head_walker; //returns walker if key matches
        }
        else 
        {
            head_walker = head_walker->_next; //If not, moves onto next node.
        }
    }
    return nullptr; //Returns null if key is never found.
}


template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this)
{
    const bool debug = false;
    if (head == nullptr) //if head is empty
    {
        node<T>* new_node = new node<T>(insert_this, nullptr, nullptr); //Creates new dynamic node 
    
        //For debugging purposes...
        if(debug)
        {
            cout << "DEBUG: new node being created when null head: ";
            cout << "HEAD: " << head << endl;
            cout << "Location: " << new_node << ", Item: " << new_node->_item <<  ", Next: " << new_node->_next << ", Prev: " << new_node->_prev << endl;
        }
        head = new_node; //Sets new node as head.
    }
    else //if there is preexisting head
    {
        node<T>* new_node = new node<T>(insert_this, head, nullptr); //Creates new dynamic node 
    
        //For debugging purposes
        if(debug)
        {
            cout << "DEBUG: new node being created: ";
            cout << "HEAD: " << head << endl;
            cout << "Location: " << new_node << ", Item: " << new_node->_item <<  ", Next: " << new_node->_next << endl;
        }
        node<T>* temp = head; //Gets temp node to hold head place
        temp->_prev = new_node; //Sets temp's prev node to new node.
        head = new_node; //Sets new node as new head
    }
    return head;
}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this)
{
    const bool debug = false;
    node<T>* insert_me; //Initializes new node.

    //For debugging purposes...
    if (debug)
    {
        cout << "DEBUG: " << endl;
        cout << "after_this: " << after_this  << ", item to insert: " << insert_this << endl;
        _print_list(head);
    }

    if (head == nullptr || after_this == nullptr) //Checks if head is null
    {
        //For debugging purposes...
        if (debug)
        {
            cout << "DEBUG: CASE: EMPTY LIST DETECTED" << endl;
        }
        _insert_head(head, insert_this); //Inserts new head.
        return head;
    }

    insert_me = new node<T> (insert_this); //Creates a new node with given item.
    node<T>* next_node = after_this->_next; //Gets the next node after after_this.
    insert_me->_prev = after_this; //Sets insert_me prev node to after_this.
    insert_me->_next = next_node; //Sets insert_me's next node.
    after_this->_next = insert_me; 

    return insert_me;
}

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this)
{
    const bool debug = false;

    node<T>* insert_me; //Initializes new_node.

    //For debugging purposes...
    if (debug)
    {
        cout << "DEBUG: " << endl;
        cout << "before_this: " << before_this << ", insert_this: " << insert_this << endl;
    }

    //Inserts new head if nullptr mark is detected or if head is detected.
    if (before_this == head || head == nullptr || before_this == nullptr)
    {
        _insert_head(head, insert_this);
    }
    else
    {
        insert_me = new node<T>(insert_this); //Creates new node with new item.
        node<T>* prev_node = before_this->_prev; //Gets previous node.
        prev_node->_next = insert_me; //Sets prev_node's next node to insert_me.

        //For debugging purposes...
        if (debug)
        {
            cout << "DEBUG: " << endl;
            cout << "prev_node: " << prev_node << ", prev_node->_next: " << prev_node->_next << ", prev_node->_prev: " << prev_node->_prev << endl;
        }

        insert_me->_next = before_this; //Sets insert_me's next node to before_this.
        insert_me->_prev = prev_node; //Sets insert_me's prev node to prev_node.

        //For debugging purposes...
        if (debug)
        {
            cout << "DEBUG: " << endl;
            cout << "insert_me: " << insert_me << ", insert_me->_next: " << insert_me->_next << ", insert_me->_prev: " << insert_me->_prev << endl;
        }

        before_this->_prev = insert_me; //Sets before_this's prev node to insert_me

        //For debugging purposes...
        if (debug)
        {
            cout << "DEBUG: " << endl;
            cout << "before_this: " << before_this << ", before_this->_next: " << before_this->_next << ", before_this->_prev: " << before_this->_prev << endl;
        }

        //For debugging purposes...
        if (debug)
        {
            cout << "DEBUG: " << endl;
            _print_list(head);
        }
    }
    return insert_me;
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this)
{
    if (prev_to_this == nullptr) //Checks if prev_to_this is null
    {
        return nullptr; //returns null
    }
    return prev_to_this->_prev; //Returns previous node.
}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this)
{
    assert (head != nullptr && delete_this != nullptr); //Checks for an empty head/invalid node.
    const bool debug = false; 
    T item_to_return = delete_this->_item; // Gets item of the delete_this node

    if (delete_this == head)
    {
        //For debugging purposes...
        if (debug)
        {
            cout << "DEBUG: CASE: HEAD: " << endl;
        }
        
        if (delete_this->_next == nullptr)
        {
            head = nullptr; //Turns head to null.
        }
        else
        {
            head = delete_this->_next; //Sets the new head.
        }
    }
    else if (delete_this->_next == nullptr) //Checks for the end of the list.
    {
        //For debugging purposes...
        if (debug)
        {
            cout << "DEBUG: CASE: LAST NODE: " << endl;
        }
        node<T>* prev_node = delete_this->_prev; //Gets prev node of delete_this.
        prev_node->_next = nullptr; //Sets previous node's next node to nullptr.
    }
    else //For every node thats not head/end of list.
    {
        //For debugging purposes...
        if (debug)
        {
            cout << "DEBUG: CASE: OTHERS: " << endl;
        }
        node<T>* prev_node = delete_this->_prev; //Gets prev node of delete_this.
        node<T>* next_node = delete_this->_next; //Gets next node of delete_this.
        prev_node->_next = next_node; //Sets previous node's next node to next_node.
        next_node->_prev = prev_node; //Sets next_node's previous node to prev_node.
    }

    //For debugging purposes...
    if (debug)
    {
        cout << "DEBUG: FINISHED CASES" << endl;
    }
    delete delete_this; //Deletes node.
    return item_to_return; //Returns the item.
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head)
{
    if (head == nullptr)
    {
        return head;
    }

    node<T>* walker = head; //Gets walker.
    node<T>* new_list = new node<T>(); //Gets new node for list.
    node<T>* new_head = new_list; //Creates a headptr
    node<T>* temp = new_list; //Creates a temp

    new_head->_item = walker->_item; //Sets new_list item to walker item
    walker = walker->_next; //Moves walker forward. 

    //Loops copying list.
    while (walker != nullptr)
    {
        new_list = new node<T>(); 
        temp->_next = new_list;
        temp = temp->_next;
        new_list->_item = walker->_item;
        walker = walker->_next;   
    }

    return new_head;
}

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src)
{
    dest = _copy_list(src); //Copies list.
    node<T>* walker = dest; //Has walker of dest
    node<T>* last_node = new node<T>(); //Creates the last node.

    //Loops to find last_node
    while (walker)
    {
        if (walker->_next == nullptr) //Once last node is found.
        {
            last_node = walker; //Gets last node
        }
        walker = walker->_next; //Moves onto next node.
    }

    return last_node;
}


//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head)
{
    const bool debug = false;

    //Loops for entire list
    while(head)
    {
        T deleted_val = _delete_node (head, head); //Deletes node.
        
        //For debugging purposes...
        if (debug)
        {
            _print_list(head);
            cout << "DEBUG: " << deleted_val << endl;
            
        }
    }
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos)
{
    const bool debug = false;
    node<T>* head_walker = head; //Gets head_walker;
    node<T>* size_walker = head; //Gets size walker to get size.
    int count = 0; //Gets count for loop
    int size_checker = 0; //Gets size of list

    //Gets size of the list
    while (size_walker != nullptr)
    {
        size_checker++; //Increments size_checker
        size_walker = size_walker->_next; //Moves onto next node
    }

    //For debugging purposes...
    if (debug)
    {
        cout << "DEBUG: size: " << size_checker << endl;
    }

    //If position is out of bounds, it will assert.
    assert (pos > 0 || pos < size_checker);

    //Gets the item at specific pos
    while (head_walker != nullptr)
    {
        if (count == pos) //If position and counter matches, loop will stop
        {
            break;
        }
        count++; //Increments counter.
        head_walker = head_walker->_next; //Increments walker.
    }
    return head_walker->_item;
}

#endif