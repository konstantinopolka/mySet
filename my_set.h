#include <vector>
#include <deque>
#include <string>
#include <type_traits>
#include <iostream>
#include <memory>
#include <sstream>
#include <algorithm> 

using namespace std;

template <typename T>
class my_set
{
public:
    my_set();
    my_set(const my_set& other);
    bool operator==(const my_set& other);
    bool operator!=(const my_set& other);
    my_set& operator=(const my_set& other);   
    void clear();

    static void swap(my_set& set1, my_set& set2) {
        using std::swap;
        swap(set1.size_index,set2.size_index);
        swap(set1.count, set2.count);
        swap(set1.actual_size, set2.actual_size);
        swap(set1.values, set2.values);
    }

    void insert(const T&);
    bool erase(const T&);
    static size_t size();
    bool empty();
    bool contains(const T&);
    
private:
    static const size_t sizes_count = 29;
    vector<size_t> sizes;
    int size_index;
    size_t actual_size;
    shared_ptr<vector<deque<T>>> values;
    size_t count;
    const double MAX_FULLNESS = 0.75;

    size_t get_index(const T&);
    void rehash(size_t new_size);
    bool too_much();
    bool too_few();
    size_t static get_hash(const T& value)
    {
        std::ostringstream oss;
        oss << value;
        string string_key = oss.str();
        size_t hash_value = hash<string>{}(string_key);
        return hash_value;
    }
};

