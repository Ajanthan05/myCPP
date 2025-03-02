// CPP Program to demonstrate make_pair()
// function in pair
#include <iostream>
#include <utility>
using namespace std;

// Driver Code
int main()
{
    pair<int, char> PAIR1;
    pair<string, double> PAIR2("GeeksForGeeks", 1.23);
    pair<string, double> PAIR3;

    PAIR1.first = 100;
    PAIR1.second = 'G';

    PAIR3 = make_pair("GeeksForGeeks is Best", 4.56);

    cout << PAIR1.first << " ";
    cout << PAIR1.second << endl;

    cout << PAIR2.first << " ";
    cout << PAIR2.second << endl;

    cout << PAIR3.first << " ";
    cout << PAIR3.second << endl;


    {
    pair<char, int> pair1 = make_pair('A', 1);
    pair<char, int> pair2 = make_pair('B', 2);

    cout << "Before swapping:\n ";
    cout << "Contents of pair1 = " << pair1.first << " "
         << pair1.second;
    cout << "Contents of pair2 = " << pair2.first << " "
         << pair2.second;
    pair1.swap(pair2);

    cout << "\nAfter swapping:\n ";
    cout << "Contents of pair1 = " << pair1.first << " "
         << pair1.second;
    cout << "Contents of pair2 = " << pair2.first << " "
         << pair2.second;
    }
    return 0;
}
