#ifndef MULTIMAP_H
#define MULTIMAP_H
#include <iostream>
using namespace std;
#include <vector>
#include "bplustree.h"
#include "../../includes/stl_utilities/vector_utilities.h"

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K())
    {
        key = k;
    }
    MPair(const K& k, const V& v)
    {
        key = k;
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist)
    {
        key = k;
        value_list = vlist;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me)
    {
        outs << print_me.key << ":" << print_me.value_list;
        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key==rhs.key);
    }

    friend bool operator !=(const MPair<K,V>& lhs, const MPair<K,V>& rhs)
    {
        return (lhs.key!=rhs.key);
    }

    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key<rhs.key);
    }

    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key<=rhs.key);
    }

    friend bool operator >= (const MPair<K,V>& lhs, const MPair<K,V>& rhs)
    {
        return (lhs.key>=rhs.key);
    }

    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key>rhs.key);
    }

    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        vector<V> temp = lhs.value_list;
        temp += rhs.value_list;
        return (MPair<K,V>(lhs.key, temp));
    }
};

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator
    {
    public:
        friend class MMap;
        Iterator(){}
        Iterator(typename map_base::Iterator it)
        {
            _it = it;
        } 

        Iterator operator ++(int unused)
        {
            Iterator copy = *this;
            _it++;
            return copy;
        }

        Iterator operator ++()
        {
            _it++;
            return *this;
        }

        MPair<K, V> operator *()
        {
            return *(_it);
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs._it == rhs._it);
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs._it!=rhs._it);
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap()
    {
        key_count = 0;
    }

    Iterator begin()
    {
        return Iterator(mmap.begin());
    }

    Iterator end()
    {
        return Iterator(mmap.end());
    }

    int size() const 
    {
        return key_count;
    }

    bool empty() const
    {
        return (key_count==0);
    }

    vector<V>& operator[](const K& key)
    {
        //Gets pair to check for key...
        MPair<K,V> check_Pair(key);

        //If pair is found
        if(mmap.contains(check_Pair)) 
        {
            return mmap.get(check_Pair).value_list;
        }
        else //if pair is not found
        {
            mmap.insert(check_Pair);
            key_count++;
            return mmap.get(check_Pair).value_list;
        }
    }

    vector<V>& at (const K& key)
    {
        //Gets pair to check for key...
        MPair<K,V> check_Pair(key);
        
        //Checks for pair
        if(mmap.contains(check_Pair))
        {
            return mmap.get(check_Pair).value_list;
        }
    }

    void insert(const K& k, const V& v)
    {
        //cout << "INSERT IS HAPPENING" << endl;
        //Get pair for checking
        MPair<K,V> check_Pair(k,v);

        //If pair is not found in mmap
        if(!mmap.contains(check_Pair))
        {
            mmap.insert(check_Pair);
            key_count++;
        }
        else //If it is found
        {
            mmap.get(check_Pair).value_list.push_back(v);
        }
    }

    void erase(const K& key)
    {
        //Gets pair to remove
        MPair<K,V> pair_to_Erase(key, V());

        //Removes the pair
        mmap.remove(pair_to_Erase);
        key_count--;
    }

    void clear()
    {
        mmap.clear_tree();
        key_count = 0;
    }

    bool contains(const K& key) const
    {
        return mmap.contains(key);
    }

    vector<V> &get(const K& key)
    {   
        //Gets pair for checking
        MPair<K,V> check_Pair(key);

        //If pair is found
        if(mmap.contains(check_Pair))
        {
            return mmap.get(check_Pair).value_list;
        }
        
        //Inserts the pair into mmap
        mmap.insert(check_Pair);
        key_count++;

        return mmap.get(check_Pair).value_list;
    }

    Iterator find(const K& key)
    {
        //Gets pair for checking
        MPair<K,V> check_Pair(key, V());
        
        typename map_base::Iterator find_me = mmap.find(check_Pair);

        //if find_me is not equal to the end...
        if(find_me != mmap.end())
        {
            return Iterator(find_me);
        }
        else
        {
            return mmap.end();
        }
    }
    int count(const K& key)
    {
        if(mmap.contains(key))
        {
            return mmap.get(key).value_list.size();
        }
        else
        {
            return 0;
        }
    }

    Iterator lower_bound(const K& key)
    {

        typename map_base::Iterator lower_b = mmap.lower_bound(key);
        return lower_b;
    }

    Iterator upper_bound(const K& key)
    {
        typename map_base::Iterator upper_b =  mmap.upper_bound(key);
        return upper_b;
    }

    bool is_valid()
    {
        return true;
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me)
    {
        outs << print_me.mmap << endl;
        return outs;
    }

    void print_lookup()
    {
        Iterator it = mmap.begin();
        Iterator end = mmap.end();
        for(; it!=end; it++)
        {
            cout << *it << endl;
        }
    }

private:
    int key_count;
    BPlusTree<MPair<K, V> > mmap;
};
#endif
