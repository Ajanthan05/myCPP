#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include <utility> // For std::swap
#include <map>

using namespace std;

void Functions_for_Character_Classification() {
    char ch = 'A';

    cout << "isalpha('A'): " << isalpha(ch) << endl;  // Output: 1 (true)
    cout << "isdigit('A'): " << isdigit(ch) << endl;  // Output: 0 (false)
    cout << "islower('A'): " << islower(ch) << endl;  // Output: 0 (false)
    cout << "isupper('A'): " << isupper(ch) << endl;  // Output: 1 (true)

    char upper = 'A';
    char lower = 'z';

    cout << "tolower('A'): " << (char)tolower(upper) << endl; // Output: 'a'
    cout << "toupper('z'): " << (char)toupper(lower) << endl; // Output: 'Z'

    /* 
    bool isalpha(int ch)	Checks if ch is an alphabetic letter (A-Z, a-z).
    bool isdigit(int ch)	Checks if ch is a digit (0-9).
    bool isalnum(int ch)	Checks if ch is an alphanumeric character (A-Z, a-z, 0-9).
    bool islower(int ch)	Checks if ch is a lowercase letter (a-z).
    bool isupper(int ch)	Checks if ch is an uppercase letter (A-Z).
    bool isspace(int ch)	Checks if ch is a whitespace character (space, tab, newline, etc.).
    bool ispunct(int ch)	Checks if ch is a punctuation character (e.g., .,;:!?).
    */
}

void reverse() {
    string s = "TrEe";
    int st = 0;
    int end = s.size()-1;

    while(st < end) {
        swap(s[st], s[end]);
        st++;
        end--;
    }

    cout << s << "\n";
    cout << s.size() << "\n";  // ans: 4
}

void reverse_o() {
    string s = "TrEe";

    for(int st=0, end=s.size()-1; st < end; st++,end--) {
        swap(s[st], s[end]);
    }

    cout << s << "\n";
}

bool isPalindrome() {
    string s = "Trert";

    for(int st=0, end=s.size()-1; st < end; st++,end--) {
        // if(s[st] != s[end]) return false; 
        if(tolower(s[st]) != tolower(s[end])) return false; 
    }
    
    return true;
}


bool rotation_of(string s1, string s2) {
    if(s1.size() != s2.size()) return false;

    string tmp = s1 + s2;

    return (tmp.find(s2) != string::npos);
}

void frequency(const string s) {
    map<char, int> mpp;

    std::for_each(s.begin(), s.end(), [&mpp](char x){mpp[x]++;});

    for (const auto &pair : mpp) {
        cout << pair.first << ": " << pair.second << " ";
    }
    cout << "\n";
}


int main() {

    reverse();
    cout << isPalindrome() << "\n";

    string s = "AruNaa";
    frequency(s);
    return 0;
}