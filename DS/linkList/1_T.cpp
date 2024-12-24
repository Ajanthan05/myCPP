#include <iostream>
#include <vector>

template <class T>
struct Node
{
public:
    T data;
    Node<T> *next;

    Node(T _data, Node<T> *_next)
    {
        data = _data;
        next = _next;
    }

    Node(T _data)
    {
        data = _data;
        next = nullptr;
    }
};

template <class T>
Node<T>* convertArr2LL(std::vector<T> &arr)
{
    Node<T>* head = new Node<T>(arr[0]);
    Node<T>* mover = head;
    for (int i = 1; i<arr.size(); i++) {
        Node<T>* temp = new Node<T>(arr[i]);
        mover->next = temp;
        mover = temp;  // mover go to next
    }
    return head;
}

template <class T>
void printLL(Node<T>* head) {
    Node<T>* temp = head;
    while(temp) {
        std::cout << temp->data << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

template <class T>
int checkIfPresent(Node<T>* head, T val) {
    Node<T>* temp = head;
    while(temp) {
        if (temp->data == val) return 1;
        temp = temp->next;
    }
    return 0;
}

int main() {
    std::vector<int> arr = {1,2,3,4};
    // Node* a = new Node(arr[0], nullptr); // new give pointer to the memory location
    Node<int> a = Node<int>(arr[0], nullptr); // we are creating an object
    std::cout << a.next << std::endl;

    Node<int>* head = convertArr2LL(arr);
    printLL(head);

    std::cout << "Val is " << (checkIfPresent(head, 4) ? "present\n" : "not present\n");
}