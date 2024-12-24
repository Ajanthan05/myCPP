#include <iostream>
using namespace std;

class String
{
private:
    char* res;
    unsigned int len;

public:
    String() : res(nullptr), len=0 
    {

    }

// Parameterised constructor
    String(const char* ch)   // we always passing constant string
    {
        len = strlen(ch);
        res = new char[len +1];  // for /0 to mark the ending of the string
        strcpy(res, ch);
    }

// Coppy constructor
    String(const Strinng& str) // CONSTANT for not to change str accedently, with out & , then u need to have copy constructor
    {
        len = str.len;  // same class we can access private member
        res = new char[len + 1];
        strcpy(res, str.res);
    }

// Move Constructor
    String(const String&& str)  // && is rValue reference => ur passing something as temporrary object
    {
        len = str.len;
        res = str.res;
        str = nullptr;
        str.len = 0;
    }

// Coppy assignment operator
/* Here object is already defined and we are copying from other string 
str3 = str1; // copy assignment operator
After this i call 
str3 = "hello";  it call coppy assignment operator
*/

    String& operator=(const String& str)
    {
        // to stop doing str3 = str3;
        if(this != &str)  // this is a pointer whish is refereing to the present object who has called. here String str3 = str1; this is str3
        {
            /* str3 = str1;
        here str3 must have some memory if we didn't deleat it, it will lead to memory leak */
            delete[] res;  // [] is used to deleat the array of memory
            len = str.len;
            res = new char[len + 1];
            strcpy(res, str.res);

            // char* temp = this->res;
            // len = str.len;
            // res = new char[len + 1];
            // strcpy(res, str.res);
            // delete[ temp;]

        }
        return *this;
    }

    /* 
    What is CAS (Copy And Swap)
    str3=str1;  now String dosen't have reference, in str, str1 copy will present (it call copy constructor)*/
    String& operator=(String str)
    {
        swap(this, str);  
        return *this;    // str is temperary it will det deleted
    }

    swap(String& str1, String& str2)
    {
        std::swap(str1.res, str2.res);
        std::swap(str1.len, str2.len);
    }

    unsigned int length()
    {
        return len;
    }

    /* Friend function => can access the private member of class outside */
    friend ostream& operator<<(ostream& out, String& str);

    friend istream& operator>>(istream& out, String& str);

    ~String()
    {
        if(res)
        {
            delete[] res;
            res = nullptr;
            len = 0;
        }
    }
};

/* cout as an object and str1 as parameter to the object => for that we have to go to cout class and implement there*/

/* here << as operator function and cout and str1 as parameters*/
ostream& operator<<(ostream& out, const String& str)
{
    out<<str.res;  // here we cannot access private member outside the ckass if not frind function
    return out;   // So that we can do chain like cout<<srt3<<srt2<<srt1;
}

istream& operator>>(istream& in, const String& str)
{
    in>>str.res;   
    return in;   
}



int main() {

    String str1; // default constructor
    String str2 = "hello"; // parameterised constructor
    String str3 = str1;  // copy constructor
    str3 = str1; // copy assignment operator
    cout<<str1 // overload >>
    cin>>str1;   // ouerload <<

    string str4(str1); // this will take cair by copy constructor

    return 0;
}