#include <iostream>
#include <vector>

struct Node
{
public:
    int data;
    Node* next;
    Node* back;

    Node(int _data, Node* _next, Node* _back)
    {
        data = _data;
        next = _next;
        back = _back;
    }

    Node(int _data)
    {
        data = _data;
        next = nullptr;
        back = nullptr;
    }
};


Node* convertArr2DLL(std::vector<int> &arr) {
    Node* head = new Node(arr[0]);
    Node* prev = head;
    for (int i=1; i<arr.size(); i++) {
        Node* tmp = new Node(arr[i], nullptr, prev);
        prev->next = tmp;
        prev = tmp;
    }
    return head;
}

void printDLL(Node* head) {
    Node* temp = head;
    while(temp) {
        std::cout << temp->data << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

Node* deleteHead(Node* head) {
    if (head == NULL || head->next == NULL) return NULL;

    Node* prev = head;
    head = head->next;
    head->back = nullptr;
    prev->next = nullptr;
    delete prev;
    return head;
}

Node* deleteTail(Node* head) {
    if (head == NULL || head->next == NULL) return NULL;

    Node* tail = head;
    // Node* prev = nullptr;
    while (tail->next != NULL) {
        // prev = tail;     // don't need to store previous
        tail = tail->next; 
    }
    Node* newTail = tail->back;
    newTail->next = nullptr;
    // prev->next = nullptr;
    tail->back = nullptr;
    delete tail;
    return head;
}

Node* deleteKth(Node* head, int k) {
    if (head == NULL) return NULL;
    Node* tmp = head;
    int cnt = 0;
    while(tmp != NULL) {
        cnt++;
        if(cnt == k) break;
        tmp = tmp->next;
    }
    Node* prev = tmp->back;
    Node* front = tmp->next;

    if(prev == NULL && front == NULL) {
        delete head;
        return NULL;
    }
    else if(front == NULL) {
        return deleteHead(head);
    }
    else if(front == NULL) {
        return deleteTail(head);
    }
    else {
        prev->next = front;
        front->back = prev;

        /* Debugging or Future Modifications:
If you later introduce changes to the code where tmp might still be 
accessed after delete tmp (e.g., due to a bug or oversight), setting next and back to nullptr helps prevent accidental use of dangling pointers.*/
        // tmp->next = nullptr;
        // tmp->back = nullptr;
        
        delete tmp;
        return head;
    }
}

Node* insertHead(Node* head, int val) {
    // if (head == NULL) return NULL;
    // Node* tmp = head;            // No need

    Node* nhead = new Node(val, head, nullptr);
    head->back = nhead;
    return nhead;
}

Node* insertTail(Node* head, int val) {
    if (head->next == NULL) return insertHead(head, val);
    else if (head == NULL) return new Node(val, nullptr, nullptr);

    Node* tail = head;

    while(tail->next != NULL) {
        tail = tail->next;
    }
    Node* prev = tail->back;
    Node* nNode = new Node(val, tail, prev);
    prev->next = nNode;
    tail->back = nNode;
    return head;
}

Node* insertBeforeK(Node* head, int k, int val) {
    if (k == 1) return insertHead(head, val);

    Node* tmp = head;
    int cnt = 0;
    while(tmp != NULL) {
        cnt++;
        if(cnt == k) break;
        tmp = tmp->next;
    }
    if(cnt < k) return head;
    else if(tmp->next = NULL) return insertTail(head, val);
    Node* prev = tmp->back;
    Node* nNode = new Node(val, tmp, prev);
    prev->next = nNode;
    tmp->back = nNode;
    return head;
}

Node* reverseDLL(Node* head) {
    if (head == NULL || head->next == NULL) return head;

    Node* last = nullptr;
    Node* current = head;

    while(current != NULL) {
        last = current->back;
        current->back = current->next;
        current->next = last;

        current = current->back;
    }
    return last->back;
}

Node* add2LL(Node* head1, Node* head2) {
    Node* dummyHead = new Node(-1);
    Node* curr = dummyHead;
    int carry = 0;
    while(head1 != NULL || head2 != NULL) {
        int sum = carry;
        if(head1) sum += head1->data;
        if(head2) sum += head2->data;
        Node* newNode = new Node(sum % 10);
        carry = sum / 10;

        curr->next = newNode;
        cut = cur->next; 
        if(head1) head1 = head1->next;
        if(head2) head2 = head2->next;
    }
    if(carry) {
        curr->next = new Node(carry);
    }
    return dummyHead->next;
}

int main() {

    std::vector<int> arr = {1,2,3,4};
    Node* head = convertArr2DLL(arr);
    printDLL(head);

    deleteTail(head);
    printDLL(head);

    head = insertHead(head, 5);
    printDLL(head);

    head = insertTail(head, 0);
    printDLL(head);

    head = insertBeforeK(head, 6, 0);
    printDLL(head);

    head = reverseDLL(head);
    printDLL(head);

    return 0;
}