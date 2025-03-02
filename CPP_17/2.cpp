#include <iostream>
#include <vector>
#include <execution>
#include <algorithm>
#include <future>
#include <chrono>
#include <thread>

#include <filesystem>

using namespace std;

int sum(int a, int b) {
    cout << "Adding 2 numbers" << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return a + b;
}

namespace fs = std::filesystem;

struct Person {
    int age;
    string address;
    long long mobilenumber;
};

int main() {
    
    /*  1 STRUCTUERE BINDINGS */
    Person p = {25, "Jaffna", 89856166666};
    auto [age, addr, mob] = p;
    cout << "age: " << age << "\n";

    /* 2 pARALLEL ALGORITHM */
    vector<int> v = {5,4,85,96,69};

    std::sort(std::execution::par, v.begin(), v.end());

    for(auto& itr : v) {
        cout << itr << ", ";
    }
    cout << "\n";

    // STD FEATURE & STD ASYNC
    future<int> ft = std::async(sum, 8, 6);
    // Do some task
    int var = ft.get();

    /*  4 STD FILE SYSTEM */
    fs::create_directory("tempdir");
    // fs::copy_file("src.txt", "des.txt", fs::copy_options::overwrite_existing);

    for(auto& itr : fs::directory_iterator("tempdir")) {
        cout << "name: " << itr.path() << endl;
    }

    /*  5 AUTO */


    return 0;
}