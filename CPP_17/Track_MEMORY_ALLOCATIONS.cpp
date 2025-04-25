#include <iostream>
#include <memory>


using namespace std;

struct AllocationMetrics {
    uint32_t TotalAllocated = 0;
    uint32_t TotalFreed = 0;

    uint32_t currentUseage() { return TotalAllocated - TotalFreed; }
};

static AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size) {
    // cout << "Allocating: " << size << "\n";

    s_AllocationMetrics.TotalAllocated += size;

    return malloc(size); 
}

void operator delete(void* memory, size_t size) {
    // cout << "Freeing: " << size << "\n";
    s_AllocationMetrics.TotalFreed += size;

    free(memory);
}

struct Object {
    int x, y, z;
};

static void PrintMemoryUsage() {
    cout << "Memory Usages: " << s_AllocationMetrics.currentUseage() << "\n";
}

int main() {
    PrintMemoryUsage();
    string s = "cherno";

    PrintMemoryUsage();
    Object* obj = new Object;

    {
        PrintMemoryUsage();
        unique_ptr<Object> p = make_unique<Object>();
        PrintMemoryUsage();
    }
    PrintMemoryUsage();
    return 0;
}