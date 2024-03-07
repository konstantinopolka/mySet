#pragma once
#include "my_set.h"


template<typename T>
my_set<T>::my_set() :
    sizes
{
    5, 11, 23, 97, 193, 389, 769, 1543, 3072, 3079, 12289, 24593,
    49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
    50331653, 100663319, 201326611, 402653189, 805306457, 1610612736, 2147483629
}
{
    size_index = 0;
    actual_size = sizes[size_index];
    values = make_shared<vector<deque<T>>>(actual_size);
    count = 0;

}

template<typename T>
inline my_set<T>::my_set(const my_set& other) : 
    sizes(other.sizes), 
    size_index(other.size_index),
    actual_size(other.actual_size),
    values(make_shared<vector<deque<T>>>(actual_size)),
    count(other.count)
{
    for (auto i = other.values->begin(); i != other.values->end(); i++) {
        for (auto j = i->begin(); j != i->end(); j++) {
            size_t index = get_index(*j);
            values->at(index).push_back(*j);
        }
    }
       
}

template<typename T>
inline bool my_set<T>::operator==(const my_set& other)
{
    if(this->count != other.count || this->values->size() != other.values->size())
        return false;

    for (size_t i = 0; i < values->size(); i++) {
        if (this->values->at(i).size() != other.values->at(i).size())
            return false;
        for (size_t j = 0; j < values->at(i).size(); j++)
            if (this->values->at(i)[j] != other.values->at(i)[j])
                return false;
    }
    return true;
}

template<typename T>
inline bool my_set<T>::operator!=(const my_set& other)
{
    return !operator==(other);
}

template<typename T>
inline my_set<T>& my_set<T>::operator=(const my_set<T>& other)
{
    clear();
    size_index = other.size_index;
    actual_size = other.actual_size;
    count = other.count;
    values = make_shared<vector<deque<T>>>(actual_size);
    for (size_t i = 0; i < other.values->size(); i++) {
        for (auto j : other.values->operator[](i)) {
            auto toInsert = j;
            size_t index = get_index(toInsert);
            values->operator[](index).push_back(toInsert);
        }
    }

    return *this;
    
}

template<typename T>
inline void my_set<T>::clear()
{
    count = 0;
    size_index = 0;
    actual_size = sizes[size_index];
    values = make_shared<vector<deque<T>>>(actual_size);
}

template<typename T>
inline void my_set<T>::insert(const T& value)
{
    if (contains(value))
        return;
    size_t index_to_insert = get_index(value);
    values->at(index_to_insert).push_back(value);
    count++;
    if (too_much())
        rehash(sizes[++size_index]);
}

template<typename T>
inline bool my_set<T>::erase(const T& value)
{
    size_t index = get_index(value);
    auto& list = values->at(index);
    auto it = std::find(list.begin(), list.end(), value);
    if (it != list.end()) {
        list.erase(it);
        count--;
        if (too_few())
            rehash(sizes[--size_index]);
        return true;
    }
    return false;
}

template<typename T>
inline size_t my_set<T>::size()
{
    return count;
}

template<typename T>
inline bool my_set<T>::empty()
{
    return count == 0;
}

template<typename T>
inline bool my_set<T>::contains(const T& value)
{
    size_t index = get_index(value);
    for (auto& i : values->at(index))
        if (i == value)
            return true;
    return false;
}

template<typename T>
inline size_t my_set<T>::get_index(const T& value)
{
    return get_hash(value) % actual_size;
}

template<typename T>
void my_set<T>::rehash(size_t new_size)
{
    actual_size = new_size;
    shared_ptr<vector <deque <T> >> new_values = make_shared<vector <deque <T> >>(actual_size);

    for (size_t i = 0; i < values->size(); i++) {
        for (auto j : values->operator[](i)) {
            auto toInsert = j;
            size_t index = get_index(toInsert);
            new_values->operator[](index).push_back(toInsert);
        }
    }

    values = new_values;
}

template<typename T>
bool my_set<T>::too_much()
{
    return (double)count / actual_size >= MAX_FULLNESS;
    // Implementieren Sie die too_much-Funktion
}

template<typename T>
bool my_set<T>::too_few()
{
    if (size_index >= 1) {
        if ((double)count / sizes[size_index - 1] < MAX_FULLNESS)
            return true;
    }
    return false;
    // Implementieren Sie die too_few-Funktion
}