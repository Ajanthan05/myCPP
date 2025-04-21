#include <iostream>
#include <vector>
#include <climits>
#include <string>

using namespace std;

struct Node 
{
private:
    Node* links[26];
    int cp;     // Count prefix
    int ew;     // End with

public:
    Node() {
        cp = 1;
        ew = 0;
        for(int i=0; i<26; i++) links[i] = nullptr;
    }

    
};