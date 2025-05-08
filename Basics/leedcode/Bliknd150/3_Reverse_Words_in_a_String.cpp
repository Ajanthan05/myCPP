#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

string reverseWords(string s) {
    std::stringstream ss(s);
    std::string word;
    std::vector<std::string> words;

    // Extract words and ignore extra spaces
    while (ss >> word) {
        words.push_back(word);
    }

    // Reverse the order of words
    // std::reverse(words.begin(), words.end());

    // Join them with a single space
    std::string result;
    for (int i = words.size()-1; i >= 0; i--) {
        result += words[i];
        if (i != 0) {
            result += ' ';
        }
    }

    return result;
}

/*  This creates a string stream from the input string s.

A stringstream lets us read words one by one, automatically skipping extra spaces
This loop reads each word from the stringstream using the >> operator.

The >> operator automatically skips any whitespace (including multiple spaces), so you get clean words.

Each word is stored in the words vector.
*/


/* more optimal than previous

But in practice, this version:

Accesses the vector in reverse (hurts cache locality).

Has a conditional (if (i != 0)) inside the loop for every iteration.

In some implementations, may cause branch prediction penalties or suboptimal CPU pipelining.

📊 Why It May Be Slower in Practice
Factor	With std::reverse + forward loop	Without std::reverse + reverse loop
Cache performance	Better (sequential access)	Worse (backward access)
Loop logic	Simpler	Slightly more branches
Readability	Cleaner	Slightly more error-prone
Performance	Slightly faster in most cases	Slightly slower in some cases

✅ Summary
Even though both approaches are O(n) in theory:

The std::reverse + forward loop is more performant in practice due to better memory access patterns and simpler loop structure.

The reverse loop introduces subtle inefficiencies that add up, especially with larger datasets.
*/
string reverseWords(string s) {
    std::stringstream ss(s);
    std::string word;
    std::vector<std::string> words;

    // Extract words and ignore extra spaces
    while (ss >> word) {
        words.push_back(word);
    }

    // Reverse the order of words
    std::reverse(words.begin(), words.end());

    // Join them with a single space
    std::string result;
    for (size_t i = 0; i < words.size(); ++i) {
        result += words[i];
        if (i != words.size() - 1) {
            result += ' ';
        }
    }

    return result;
}

/*Optimal*/
string reverseWords(string s) {
    vector<string> a;
    int n = s.length();
    string word = "";
    for(char c : s){
        if(c == ' '){
            if(!word.empty()){
                a.push_back(word);
                word = "";
            }
        }
        else 
            word += c;
    }
    if(!word.empty())
        a.push_back(word);
    reverse(a.begin(),a.end());
    s = "";
    int m = a.size();
    for(int i=0;i<m;i++)
    {
        s += a[i];
        if(i<m-1)
            s+=' ';
    }
    return s;
}

int main() {

    string s = "the sky is blue";
    cout << reverseWords(s) << "\n";

    return 0;
}