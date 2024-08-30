#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include "../../includes/bplustree/btree_array_funcs.h"
#include <iostream>
#include <iomanip>


template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0): it(_it), key_ptr(_key_ptr){}

        T operator *() const
        {
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used) //After
        { 
            Iterator copy(it, key_ptr);
            ++*this;
            return copy;
        }
        Iterator operator++() //Before
        { 
            if(key_ptr<it->data_count-1){
                key_ptr++;
            }else{
                it = it->next;
                key_ptr = 0;
            }
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return ((lhs.key_ptr == rhs.key_ptr) && (lhs.it==rhs.it));
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return ((lhs.it != rhs.it));
        }
        void print_Iterator()
        {
            cout << "ITERATOR PRINT: ";
            if (it)
            {
                print_array(it->data, it->data_count, key_ptr);
            }
            else
            {
                cout << "ITERATOR == NULL, key_ptr: " << key_ptr << endl;
            }
        }
        bool is_null(){return !it;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false)
    {
        data_count = 0;
        child_count = 0;
        dups_ok = dups;
        next = nullptr;
    }

    BPlusTree(T *a, int size, bool dups = false){
        data_count = 0;
        child_count = 0;

        for(int i=0; i<size; i++)
        {
            insert(a[i]);
        }

        dups_ok = dups;
    }

    //BIG THREE FUNCTIONS

    BPlusTree(const BPlusTree<T>& other)
    {
        data_count = 0;
        child_count = 0;
        dups_ok = other.dups_ok;
        BPlusTree* last_leaf = nullptr;
        copy_tree(other, last_leaf);
    }

    ~BPlusTree()
    {
        clear_tree();
    }
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS)
    {
        if(this == &RHS)
        {
            return *this;
        }

        dups_ok = RHS.dups_ok;
        
        this->clear_tree();
        BPlusTree* last_leaf = nullptr;
        this->copy_tree(RHS, last_leaf);
        
        return *this;
    }
    
    void copy_tree(const BPlusTree<T>& other) //copy other into this object
    {
        BPlusTree<T>* last_leaf = nullptr;
        copy_tree(other, last_leaf);
    }  

    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node)
    {
        if(this == &other) //Checks if this is equal to other...
        {
            return;
        }

        copy_array(data, other.data, data_count, other.data_count);
        child_count = other.child_count;

        if(other.is_leaf() == false)
        {
            for(int i = 0; i < other.child_count; i++)
            {
                subset[i] = new BPlusTree(dups_ok);
                subset[i]->copy_tree(*other.subset[i], last_node);
            }
        } 
        else 
        {
            if(last_node != nullptr)
            {
                last_node->next = this;
            }
            last_node = this;
        }
    }

    void insert(const T& entry) //insert entry into the tree
    {
        loose_insert(entry);

        if(data_count > MAXIMUM) //CHECKS FOR EXCESS
        { 
            BPlusTree* newnode = new BPlusTree;
            copy_array(newnode->data, data, newnode->data_count, data_count); 

            if(child_count > 0) //Children exists...
            {
                copy_array(newnode->subset, subset, newnode->child_count, child_count); 
            }

            subset[0] = newnode;
            data_count = 0;
            child_count = 1;
           
            fix_excess(0);
        }

    }   

    void remove(const T& entry) //remove entry from the tree
    {
        if(data_count == 0) //When node is empty...
        { 
            return;
        }

        loose_remove(entry);
        

        if(data_count < MINIMUM && child_count > 0) //Checks root shortages
        {
            //Retrieves location of subset...
            BPlusTree* temp = subset[0]; 

            copy_array(data, temp->data, data_count, temp->data_count);   
            copy_array(subset, temp->subset, child_count, temp->child_count);

            //Clears temp...
            temp->data_count = 0;
            temp->child_count = 0;
            delete temp;

        }
    }   

    void clear_tree() //clear this object (delete all nodes etc.)
    {
        for(int i = 0; i < child_count; i++)
        {
            if(is_leaf())
            {
                return;
            }

            //Clears subset
            subset[i]->clear_tree();
            delete subset[i];
            subset[i] = nullptr;
            
        }

        //Resets count...
        data_count = 0;
        child_count = 0;
    }             

    bool contains(const T& entry) const //true if entry can be found
    {
        int first_index = first_ge(data, data_count, entry); //Gets the first available index
        bool checker = (data[first_index] == entry && first_index < data_count); //Checks for index and entry being found

        if(checker) //if entry was found
        {
            return true;
        }
        else if(is_leaf()) 
        {
            return false;
        } 
        else 
        {
            return subset[first_index]->contains(entry);
        }
    } 

    T& get(const T& entry) //return a reference to entry
    {
        if(contains(entry) == false) //If entry is not found...
        {
            insert(entry); //inserts entry
        }

        return get_existing(entry); //gets entry
    }              

    const T& get(const T& entry)const //return a reference to entry
    {
        assert(contains(entry));
        return get_existing(entry);
    }   

    T& get_existing(const T& entry) //return a reference to entry
    {
        int first_index = first_ge(data, data_count, entry);
        bool checker = (data[first_index] == entry && first_index < data_count);

        if(checker) //If found
        {
            if(is_leaf())
            {
                return data[first_index]; //done
            }
            else
            {
                return subset[first_index + 1]->get_existing(entry);
            }
        } 
        else 
        {
            if(is_leaf())
            { 
                assert(checker);
            } 
            else 
            {
                return subset[first_index]->get_existing(entry);
            }
        }
    }     

    Iterator find(const T& key) //return an iterator to this key.
    {
        int first_index = first_ge(data, data_count, key);
        bool checker = (data[first_index] == key && first_index < data_count);

        if(checker) //if found
        {
            if(is_leaf())
            {
                return Iterator(this, first_index);
            } 
            else 
            {
                return subset[first_index+1]->find(key);
            }
        } 
        else
        {
            if(is_leaf())
            {
                return Iterator(nullptr);
            } 
            else 
            {
                return subset[first_index]->find(key);
            }
        }
    }         
                                         
    Iterator lower_bound(const T& key) //return first that goes NOT BEFORE key entry or next if does not exist: >= entry
    {
        if(contains(key)) //If key is found
        {
            return find(key);
        } 
        else 
        {
            return upper_bound(key);
        }
    }  
                                    
    Iterator upper_bound(const T& key) //return first that goes AFTER key exist or not, the next entry  >entry
    {
        int first_index = first_ge(data, data_count, key);
        bool checker = (data[first_index]==key&&first_index<data_count);
        if(checker)
        {
            if (is_leaf())
            {
                Iterator up(this, first_index);
                up++;
                return up;
            } 
            else
            { 
                return subset[first_index + 1]->upper_bound(key);
            }
        } 
        else 
        {
            if (is_leaf())
            {
                Iterator up(this, first_index);
                if(first_index == data_count)
                {
                    return Iterator(this->next, 0);
                }
                return up;
            } 
            else 
            {
                return subset[first_index]->upper_bound(key);
            }
        }
    }  
                                         

    int size() const;                    //count the number of elements

    bool empty() const{return data_count == 0;}                  //true if the tree is empty

    void print_tree(int level = 0, ostream &outs=cout) const
    {
        if(is_leaf() == false)
        {
            subset[child_count-1]->print_tree(level + 1);
        }

        outs<<setw(6*level);

        for(int i = data_count - 1; i >= 0; i--)
        {
            cout << "|" << data[i] << "| " << level << endl;
            if(is_leaf() == false)
            {
                subset[i]->print_tree(level+1);
            }
            outs<<setw(6*level);
        }
    }
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid()
    {
        return true;
    }

    string in_order()
    {
        string string_to_return = ""; //return string...
        if(is_leaf() == false) //If not leaf
        { 
            for(int i = 0; i < data_count; i++)
            {
                string_to_return += subset[i]->in_order();
                string_to_return += to_string(data[i])+"|";
            }
            string_to_return += subset[data_count]->in_order();
        } 
        else //Leaf case
        { 
            for(int i = 0; i < data_count; i++)
            {
                string_to_return += to_string(data[i])+"|";
            }
        }
        return string_to_return; 
    }

    string pre_order()
    {
        string string_to_return = ""; //string return val
        if(is_leaf() == false) //not leaf
        { 
            for(int i=0; i<data_count; i++)
            {
                string_to_return += to_string(data[i])+"|";
                string_to_return += subset[i]->pre_order();
                
            }

            string_to_return += subset[data_count]->pre_order();
        } 
        else //leaf case
        { 
            for(int i=0; i<data_count; i++)
            {
                string_to_return += to_string(data[i])+"|";
            }
        }
        return string_to_return; 
    }

    string post_order()
    {
        string string_to_return = ""; //return string val
        if(is_leaf() == false) //is not a leaf
        { 
            string_to_return+=subset[0]->post_order();

            for(int i = 0; i < data_count; i++)
            {
                string_to_return += subset[i+1]->post_order();
                string_to_return += to_string(data[i])+"|";
            }
        } 
        else //Leaf case
        { 
            for(int i=0; i<data_count; i++)
            {
                string_to_return += to_string(data[i])+"|";
            }
        }
        return string_to_return; 
    }

    Iterator begin()
    {
        return Iterator(get_smallest_node());
    }

    Iterator end() {return Iterator(nullptr);}

    ostream& list_keys(Iterator from = NULL, Iterator to = NULL)
    {
        if (from == NULL) from = begin();
        if (to == NULL) to = end();

        for (Iterator it = from; it != to; it++)
        {
            cout<<*it<<" ";
        }
            
        return cout;
    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;
    bool is_leaf() const {return child_count==0;}             //true if this is a leaf node

    T* find_ptr(const T& entry)
    {
        if(contains(entry) == false) //if entry is not found
        {
            return nullptr;
        } 
        else 
        {
            return get_existing(entry);
        }
    }    //return a pointer to this key.
                                         // NULL if not there.

    void loose_insert(const T& entry) //allows MAXIMUM+1 data elements in the root
    {
        int index = first_ge(data, data_count, entry);
        bool check = (index < data_count && data[index] == entry);

        if(check == false) //if entry is not found
        { 
            if(is_leaf())
            {
                insert_item(data, index, data_count, entry);
            } 
            else
            {
                subset[index]->loose_insert(entry);
                fix_excess(index);
            }
        } 
        else //if entry is found
        { 
            if(is_leaf())
            {
                if(dups_ok == false)
                {
                    data[index] = entry;
                }
                else
                {
                    data[index] = data[index] + entry;
                }
            } 
            else
            {
                subset[index+1]->loose_insert(entry);
                fix_excess(index+1);
            }
        }
    }   
                                          
    void fix_excess(int i)
    {
        if(subset[i]->data_count > MAXIMUM) //CHECKS FOR EXCES
        { 
            if(!subset[i]->is_leaf())
            { 
                T newdata[MAXIMUM+1]; //new data array for new subtree
                int size2 = 0;

                split(subset[i]->data, subset[i]->data_count, newdata, size2); 
                ordered_insert(data, data_count, subset[i]->data[subset[i]->data_count-1]); 

                //decrement data count
                subset[i]->data_count--; 

                BPlusTree* new_subsets = new BPlusTree; //creates new subtree to combine

                //Copys subset and inserts it
                copy_array(new_subsets->data, newdata, new_subsets->data_count, size2); 
                insert_item(subset, i+1, child_count, new_subsets); 
                
                if(subset[i]->child_count>(subset[i]->data_count+1)) //too many children in subset
                { 
                    split(subset[i]->subset, subset[i]->child_count, new_subsets->subset, new_subsets->child_count);
                }
            } 
            else //is leaf 
            {  
                T newdata[MAXIMUM + 1]; 
                int size2 = 0;

                //Splits data
                split(subset[i]->data, subset[i]->data_count, newdata, size2); 
                ordered_insert(data, data_count, subset[i]->data[subset[i]->data_count-1]); 
                subset[i]->data_count--; 

                //Creates new subtree
                BPlusTree* sub2 = new BPlusTree; 
                copy_array(sub2->data, newdata, sub2->data_count, size2); 
                insert_item(subset, i+1, child_count, sub2); 
                
                //inserts subset
                ordered_insert(subset[i+1]->data, subset[i+1]->data_count, data[i]); 

                //dealing with next ptrs
                BPlusTree* temp = subset[i]->next;
                subset[i]->next = subset[i+1];
                subset[i+1]->next = temp;
            }
        }
    }      

    void loose_remove(const T& entry) //allows MINIMUM-1 data elements in the root
    {
        int index = first_ge(data, data_count, entry);
        bool check = (index < data_count && data[index] == entry);

        T* search_d;
        T* search_s;

        if(check) //if entry is found
        { 
            if(is_leaf()) //if there are no children
            { 
                T deleted;
                delete_item(data, index, data_count, deleted);
            } 
            else
            {
                subset[index+1]->loose_remove(entry);
                assert(index < child_count-1);

                if(subset[index+1]->data_count < MINIMUM)
                {
                    fix_shortage(index+1);

                    search_d = search(data, data_count, entry);

                    if(search_d)
                    {
                        subset[index+1]->get_smallest(*search_d);
                    } 
                    else 
                    {
                        search_s = search(subset[index]->data, subset[index]->data_count, entry);
                        if(search_s)
                        {
                            subset[index+1]->get_smallest(*search_s);
                        }
                    }
                }
                else
                {
                    subset[index+1]->get_smallest(data[index]);
                }
            }
        } 
        else 
        {
            if(!is_leaf())
            { 
                subset[index]->loose_remove(entry);
                fix_shortage(index);
            } 
            else //not found
            {
                return; 
            }
        }
    }

    BPlusTree<T>* fix_shortage(int i) //fix shortage in child i and return the smallest key in this subtree
    {
        if(subset[i]->data_count < MINIMUM) //CHECKS FOR SHORTAGES
        { 
            if(i > 0 && (subset[i-1]->data_count > MINIMUM)) //left
            { 
                if(subset[i]->is_leaf()) //when subset is a leaf
                {
                    T item; //holds item to take out and put back in

                    detach_item(subset[i-1]->data, subset[i-1]->data_count, item);
                    insert_item(subset[i]->data, 0, subset[i]->data_count, item);

                    data[i-1] = item;
                }
                else
                {   
                    //Sinks parent 
                    T parent;
                    delete_item(data, i-1, data_count, parent);
                    ordered_insert(subset[i]->data, subset[i]->data_count, parent); 
                    
                    //Pushes left up
                    T left;
                    delete_item(subset[i-1]->data, subset[i-1]->data_count-1, subset[i-1]->data_count, left);
                    ordered_insert(data, data_count, left); 
                    
                    if(!(subset[i]->is_leaf() && subset[i-1]->is_leaf()))
                    {
                        insert_item(subset[i]->subset, 0, subset[i]->child_count, subset[i-1]->subset[subset[i-1]->child_count-1]);
                        subset[i-1]->child_count--; 
                    }
                }
                return subset[i];
            } 
            else if (i < data_count && subset[i+1]->data_count > MINIMUM) //goes from right to left
            { 
                if(subset[i]->is_leaf())
                {
                    T item;

                    delete_item(subset[i+1]->data, 0, subset[i+1]->data_count, item);
                    insert_item(subset[i]->data, subset[i]->data_count, subset[i]->data_count, item);

                    data[i] = subset[i+1]->data[0];
                }
                else 
                {   
                    //sinks parent
                    T parent;
                    delete_item(data, i, data_count, parent);
                    ordered_insert(subset[i]->data, subset[i]->data_count, parent); 

                    //moves right up
                    T right;
                    delete_item(subset[i+1]->data, 0, subset[i+1]->data_count, right);
                    ordered_insert(data, data_count, right); 

                    if(!(subset[i]->is_leaf() && subset[i+1]->is_leaf()))
                    {
                        attach_item(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset[0]);
                        delete_ptr(subset[i+1]->subset, 0, subset[i+1]->child_count);
                    }
                }
                return subset[i];
            } 
            else if(i==data_count) //merge to the left side
            { 
                if(subset[i]->is_leaf()){
                    subset[i-1]->next = subset[i]->next; //keeping next chain
                    merge(subset[i-1]->subset, subset[i-1]->child_count, subset[i]->subset, subset[i]->child_count);
                    merge(subset[i-1]->data, subset[i-1]->data_count, subset[i]->data, subset[i]->data_count);
                    subset[i]->data_count = 0;
                    subset[i]->child_count = 0; 
                    child_count--;
                    data_count--;
                }else 
                {
                    merge(subset[i-1]->subset, subset[i-1]->child_count, subset[i]->subset, subset[i]->child_count);
                    merge(subset[i-1]->data, subset[i-1]->data_count, subset[i]->data, subset[i]->data_count);

                    subset[i]->data_count = 0;
                    subset[i]->child_count = 0; 
                    child_count--;                
                    
                    //sinks parent
                    T parent;
                    delete_item(data, i-1, data_count, parent);
                    attach_item(subset[i-1]->data, subset[i-1]->data_count, parent); 
                }
            } else { //merge with right
                if(subset[i]->is_leaf())
                {
                    subset[i]->next = subset[i+1]->next; 
                    merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
                    merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
                    subset[i+1]->data_count = 0; 
                    subset[i+1]->child_count = 0;
                    
                    delete_ptr(subset, i+1, child_count);

                    T deleted;
                    delete_item(data, i, data_count, deleted);  
                } else 
                {
                    merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
                    merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);

                    subset[i+1]->data_count = 0; 
                    subset[i+1]->child_count = 0;
                    
                    delete_ptr(subset, i+1, child_count);

                    T parent; 
                    delete_item(data, i, data_count, parent);
                    ordered_insert(subset[i]->data, subset[i]->data_count, parent);
                }
            }
        }
    } 

    BPlusTree<T>* get_smallest_node()
    {
        if(is_leaf())
        {
            return this;
        } 
        else 
        {
            return subset[0]->get_smallest_node();
        }
    }
    
    void get_smallest(T& entry)
    {
        if(!is_leaf())
        {
            subset[0]->get_smallest(entry);
        } 
        else
        {
            entry = data[0];
        }
    } 

    void get_biggest(T& entry);       //entry := rightmost leaf
    void remove_biggest(T& entry);    //remove the biggest child of tree->entry
    void transfer_left(int i);        //transfer one element LEFT from child i
    void transfer_right(int i);       //transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i); //merge subset i with  i+1

};


#endif