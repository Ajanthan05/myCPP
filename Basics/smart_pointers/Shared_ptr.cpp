/*  Reference count is shared among all the copies of the shared_ptr instances 
pointing to the same object, ensuring proper memory management and deletion.

1. Initialization using a New Pointer

std::shared_ptr<T> ptr(new T());
std::shared_ptr<T> ptr = std::make_shared<T>();
2. Initialization using existing Pointer

shared_ptr<T> ptr(already_existing_pointer);
shared_ptr<T> ptr = make_shared(already_existing_pointer);

reset()	Resets the std::shared_ptr to empty, releasing ownership of the managed object.
use_count()	Returns the current reference count, indicating how many std::shared_ptr instances share ownership.
unique()	Check if there is only one std::shared_ptr owning the object (reference count is 1).
get()	Returns a raw pointer to the managed object. Be cautious when using this method.
swap(shr_ptr2)	swaps the contents (ownership) of two std::shared_ptr instances.

reset()	Resets the std::shared_ptr to empty, releasing ownership of the managed object.
use_count()	Returns the current reference count, indicating how many std::shared_ptr instances share ownership.
unique()	Check if there is only one std::shared_ptr owning the object (reference count is 1).
get()	Returns a raw pointer to the managed object. Be cautious when using this method.
swap(shr_ptr2)	swaps the contents (ownership) of two std::shared_ptr instances.

*/

// C++ program to demonstrate shared_ptr
#include <iostream>
#include <memory>
using namespace std;

class A {
public:
    void show() { cout << "A::show()" << endl; }
};

int main()
{
    // Creating a shared pointer and accessing the object
    shared_ptr<A> p1(new A);
    // Printing the address of the managed object
    cout << p1.get() << endl;
    p1->show();
  
    // Creating a new shared pointer that shares ownership
    shared_ptr<A> p2(p1);
    p2->show();
  
    // Printing addresses of p1 and p2
    cout << p1.get() << endl;
    cout << p2.get() << endl;
  
    // Returns the number of shared_ptr objects
    // referring to the same managed object
    cout << p1.use_count() << endl;
    cout << p2.use_count() << endl;
  
    // Relinquishes ownership of p1 on the object
    // and pointer becomes NULL
    p1.reset();
    cout << p1.get() << endl; // This will print nullptr or 0
    cout << p2.use_count() << endl;
    cout << p2.get() << endl;

    /*
    These lines demonstrate that p1 no longer manages an
    object (get() returns nullptr), but p2 still manages the
    same object, so its reference count is 1.
    */
    return 0;
}

struct Node {
    int data;
    shared_ptr<Node> next;

    Node(int val)
        : data(val)
        , next(NULL)
    {
    }
};

class LinkedList {
public:
    LinkedList() : head(NULL) {}

    // Insert a new node at the end of the linked list
    void insert(int val)
    {
        shared_ptr<Node> newNode = make_shared<Node>(val);
        if (!head) {
            head = newNode;
        }
        else {
            shared_ptr<Node> current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // Delete a node with a given value from the linked list
    void del(int val)
    {
        if (!head) {
            return;
        }
        if (head->data == val) {
            head = head->next;
            return;
        }
        shared_ptr<Node> current = head;
        while (current->next
               && current->next->data != val) {
            current = current->next;
        }
        if (current->next && current->next->data == val) {
            current->next = current->next->next;
        }
    }

    // Traverse and print the linked list
    void Print()
    {
        shared_ptr<Node> current = head;
        while (current) {
            cout << current->data << " -> ";
            current = current->next;
        }
        cout << "NULL" << endl;
    }

private:
    shared_ptr<Node> head;
};

int main()
{
    LinkedList linkedList;

    // Insert nodes into the linked list
    linkedList.insert(1);
    linkedList.insert(2);
    linkedList.insert(3);

    // Print the linked list
    cout << "Linked List: ";
    linkedList.Print();

    // Delete a node and print the updated linked list
    linkedList.del(2);
    cout << "Linked List after deleting 2: ";
    linkedList.Print();

    return 0;
}