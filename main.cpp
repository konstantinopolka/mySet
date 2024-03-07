#include <iostream>
#include "Header.h"


int main()
{
    my_set<int> set;
    set.insert(5);
    set.insert(3);
    cout << set.contains(3) << endl;

    my_set<int> set2;

    set2 = set;

    set2.insert(228);


    my_set<int>::swap(set, set2);

    set.clear();

}

