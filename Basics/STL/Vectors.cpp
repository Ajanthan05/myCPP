#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/*
v.insert(pos, val);              // Insert single element 
v.insert(pos, n, val);         // Insert multiple copies of an element
v.insert(pos, first, last);   // Insert range of elements 
*/

void insert() {
    vector<int> v = {1, 2, 4, 5};
    
    v.insert(v.begin() + 2, 3);

    for (auto i : v)
        cout << i << " ";

    cout << "\nInsert Multiple Copies of an Element\n";

    /* Insert Multiple Copies of an Element
    
    v.insert(pos, n, val)
    val – Value to be inserted.
    pos: Iterator to the position where val is to be inserted.
    n: Number of times val is to be inserted.
    */

    v.insert(v.begin() + 2, 2, 3);
    for (auto i : v)
        cout << i << " ";

    cout << "\nInsert the Range of Elements\n";

    /*  Insert the Range of Elements
    vec.insert(pos, first, last)

pos– Iterator to the position where range is to be inserted.
first– Iterator to the first element in the range from which the elements are to be inserted.
last– Iterator to the element one place after the last element in the range.
    

rbegin() returns a reverse iterator pointing to the last element.
rend() is the end of the reversed range.
This ensures elements are inserted in reverse order.
    */

    list<int> l = {-1, -2, -7};
    // v.insert(v.begin()+1, l.end(), l.begin());  wrong
    v.insert(v.begin()+1, l.rbegin(), l.rend());

    for (auto i : v)
        cout << i << " ";

    cout << "\n\n";
}

// The capacity of vector will not decrease automatically on removing the elements from vector.
// Decrease capacity of vector
// v.shrink_to_fit();  method to decrease the capacity of vector which makes the capacity will equal to size of vector.

int main() {
    insert();

    return 0;
}