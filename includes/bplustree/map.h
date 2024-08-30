#ifndef MAP_H
#define MAP_H
#include <iostream>
using namespace std;
#include "bplustree.h"

template <typename K, typename V>
struct Pair
{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()){
        key = k;
        value = v;
    }
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me)
    {
        outs << print_me.key << ":" << print_me.value;
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key == rhs.key);
    }
    friend bool operator !=(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key != rhs.key);
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key < rhs.key);
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key > rhs.key);
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key <= rhs.key);
    }
    friend bool operator >= (const Pair<K,V>& lhs, const Pair<K,V>& rhs)
    {
        return (lhs.key >= rhs.key);
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {  
        return rhs;
    }
};

template <typename K, typename V>
class Map
{
    public:
        typedef BPlusTree<Pair<K, V> > map_base;
        class Iterator
        {
        public:
            friend class Map;
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

            Pair<K, V> operator *()
            {
                return *_it;
            }

            friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
                return (lhs._it == rhs._it);
            }

            friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
                return (lhs._it != rhs._it);
            }
    private:
        typename map_base::Iterator _it;
};

    Map()
    {
        key_count = 0;
    }

    Iterator begin()
    {
        return Iterator(map.begin());
    }

    Iterator end()
    {
        return Iterator(map.end());
    }

    int size() const{return  key_count;}
    bool empty() const
    {
        return (key_count == 0);
    }

    V& operator[](const K& key)
    {
        Pair<K,V> check_Pair(key, V());

        if(map.contains(check_Pair))
        {
            return map.get(check_Pair).value;
        } 
        else
        {
            map.insert(check_Pair);
            key_count++;
            return map.get(check_Pair).value;        
        }
    }

    V& at(const K& key)
    {
        Pair<K,V> check_Pair(key, V());
        if(map.contains(check_Pair))
        {
            return map.get(check_Pair).value; 
        }
    }
    
    const V& at(const K& key) const
    {
        Pair<K,V> check_Pair(key, V());
        if(map.contains(check_Pair))
        {
            return map.get(check_Pair).value; 
        }
    }

    void insert(const K& k, const V& v){
        Pair<K,V> check_Pair(k, v);
        if(!map.contains(check_Pair))
        {
            key_count++;
            map.insert(check_Pair);
        }
    }

    void erase(const K& key){
        Pair<K,V> check_Pair(key, V());
        if(map.contains(check_Pair))
        {
            map.remove(check_Pair);
            key_count--;
        }
    }

    void clear()
    {
        map.clear_tree();
        key_count = 0;
    }

    V get(const K& key)
    {
        Pair<K,V> check_Pair(key, V());
        if(map.contains(check_Pair))
        {
            return map.get(check_Pair).value;
        }
    }

    Iterator find(const K& key)
    {
        Pair<K,V> check_Pair(key, V());
         typename map_base::Iterator find_Me = map.find(check_Pair);

        if(find_Me != map.end())
        {
            return Iterator(find_Me);
        } 
        else 
        {
            return map.end();
        }
    }

    bool contains(const Pair<K, V>& target) const
    {
        return map.contains(target);
    }

    Iterator lower_bound(const K& key)
    {
        typename map_base::Iterator low_bound = map.lower_bound(key);
        return low_bound;
    }

    Iterator upper_bound(const K& key)
    {
        typename map_base::Iterator up_bound = map.upper_bound(key);
        return up_bound;
    }
   
    Iterator equal_range(const K& key)
    {
        if(map.lower_bound(key) == map.upper_bound(key))
        {
            return Iterator(map.end());
        }
        else
        {
            return Iterator(map.lower_bound(key), map.upper_bound(key));
        }
    }

    bool is_valid()
    {
        return map.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me)
    {
        outs<<print_me.map<<endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};



#endif