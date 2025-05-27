#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


vector<string> splitByDelimiter(const string& str, char delimiter) {
    stringstream ss(str);
    string token;
    vector<string> result;

    while (getline(ss, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}

int main() {
    string str = "42 hello 3.14";

    stringstream ss(str);  // Create a string stream

    int x;
    string word;
    float f;

    ss >> x >> word >> f;

    cout << x << " | " << word << " | " << f << endl;
    // 42 | hello | 3.14

    
    string line = "this is a test";
    stringstream ssl(line);
    string word;

    while (ssl >> word) {
        cout << word << endl;
    }

    // Split String by Custom Delimiter (e.g., comma)
    string data = "apple,banana,grape";
    stringstream ss3(data);
    string item3;

    while (getline(ss3, item3, ',')) {
        cout << item3 << endl;
    }

    /*  If you want to find the length (number of characters) stored in a stringstream, 
    you can use the .str() method to get the underlying string and then call .size() or .length() on it.*/

    // Get the string inside the stringstream
    string content = ss.str();

    // Get the length
    int length = content.length(); // or content.size()

    cout << "Length of content in stringstream: " << length << endl;
}
