#include <iostream>
#include <set>
using namespace std;

/*
sets are associative container which stores unique elements in some sorted order. By default, it is sorted ascending 
order of the keys, but this can be changes as per requirement. It provides fast insertion, deletion and search operations.

set<T, comp> s;
comp: It is a binary predicate function that tells set how to compare two elements. It is used to sort set in custom order. 
It is optional and if not provided, set is sorted in increasing order
*/
int main() {
    
    /*
    Unique Elements → set stores only distinct elements (no duplicates).
Sorted Order → Elements are stored in ascending order by default.
Fast Lookups, Insertions & Deletions → Operations like insert(), find(), and erase() take O(log n) time due to the balanced binary search tree (typically a Red-Black Tree).

*/
    set<int> s = {3, 5, 2, 1};
    
    for (auto x : s)
        cout << x << " ";

    /* number of times an element occurs in the set container. std::set container stores unique elements, so it can only return 1 or 0*/
    cout << "\nCount" << s.count(5) << "\n";

    // Accessing first element
    auto it1 = s.begin();
    
    // Accessing third element
    auto it2 = next(it1, 2);
    
    cout << *it1 << " " << *it2;

    // Finding 3
    auto it = s.find(3);

    if (s.find(20) != s.end()) {
        cout << "20 exists in the set";
    }
    
    if (it != s.end()) cout << *it;
    else cout << "Element not Found!";

    // Deleting elements by value
    s.erase(5);
    
    // Deleting first element by iterator
    s.erase(s.begin());

    set<int, greater<int>> s2 = {5, 2, 8, 3};
    for (int num : s2) cout << num << " ";  // Output: 8 5 3 2

    return 0;
}
