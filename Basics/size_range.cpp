#include <iostream>
#include <climits>
#include <vector>

int main() {
    std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
    std::cout << "Size of long: " << sizeof(long) << " bytes" << std::endl;

    std::cout << "Size of int*: " << sizeof(int*) << " bytes" << std::endl;
    std::cout << "Size of long*: " << sizeof(long*) << " bytes" << std::endl;

    std::cout << "Max int: " << INT_MAX << ", Min int: " << INT_MIN << std::endl;
    std::cout << "Max long: " << LONG_MAX << ", Min long: " << LONG_MIN << std::endl;



    std::vector<std::vector<int>>MyVector;
    std::vector<int>temp;

    temp.push_back(1);
    temp.push_back(2);
    temp.push_back(3);
    MyVector.push_back(temp);

    std::cout << "Rows in the 2d vector: " << MyVector.size() <<
    std::endl << "Columns in the 1st row: " << MyVector[0].size() <<
    std::endl;

    system("pause");
    return 0;
}