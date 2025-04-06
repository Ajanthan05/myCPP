#include <iostream>
#include <vector>

struct Node
{
public:
    int data;
    Node* next;

    Node(int _data, Node* _next)
    {
        data = _data;
        next = _next;
    }

    Node(int _data)
    {
        data = _data;
        next = nullptr;
    }
};

Node* convertArr2LL(std::vector<int> &arr)
{
    Node* head = new Node(arr[0]);
    Node* mover = head;
    for (int i = 1; i<arr.size(); i++) {
        Node* temp = new Node(arr[i]);
        mover->next = temp;
        mover = temp;  // mover go to next
    }
    return head;
}

void printLL(Node* head) {
    Node* temp = head;
    while(temp) {
        std::cout << temp->data << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

int checkIfPresent(Node* head, int val) {
    Node* temp = head;
    while(temp) {
        if (temp->data == val) return 1;
        temp = temp->next;
    }
    return 0;
}

Node* deleteHead(Node*head) {
    if(head == NULL) return head;
    Node* temp = head;
    head = head->next;
    delete temp;
    return head;
}

Node* deleteTail(Node*head) {
    if(head == NULL || head->next == NULL) return NULL;
    Node* temp = head;
    while(temp->next->next != NULL) {
        temp = temp->next;
    }
    delete temp->next;
    temp->next = nullptr;
    return head;
}

Node* deleteK(Node*head, int k) {
    if(head == NULL) return head;
    if(k == 1) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return head;
    }
    int counter = 0;
    Node* temp = head;
    Node* prev = NULL;
    while(temp != NULL) {
        counter++;
        if(counter == k) {
            prev->next = prev->next->next;
            delete temp;
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    return head;
}

Node* deleteEl(Node *head, int el) {
    if(head == NULL) return head;
    if(head->data == el) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return head;
    }
    Node* temp = head;
    Node* prev = NULL;
    while(temp != NULL) {
        if(temp->data == el) {
            prev->next = prev->next->next;
            delete temp;
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    return head;
}

Node* insertHead(Node* head, int val) {
    // Node* temp = head;
    // head = new Node(val, temp);
    // return head;

    // Node* temp = new Node(val, head);
    return new Node(val, head);
}

Node* insertTail(Node* head, int val) {
    if (head == NULL) return new Node(val);
    Node* tmp = head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = new Node(val);
    return head;
}

Node* insertKth(Node* head, int el, int k) {
    if (head == NULL) {
        if(k == 1) return new Node(el);
        return head;
    }
    if (k == 1) return new Node(el, head);

    int counter = 0;
    Node* tmp = head;
    while(tmp != NULL) {
        counter++;
        if (counter == (k-1)) {
            tmp->next = new Node(el, tmp->next);
            break;
        }
        tmp = tmp->next;
    }
    return head;
}

/* insert a new node before the value */
/*
Node* insertBeforeValue(Node* head, int el, int val) {
    if (head == NULL) {
        return head;
    }
    if (head->data == val) return new Node(el, head);

    Node* tmp = head;
    Node* prev = NULL;
    while(tmp != NULL) {
        if (tmp->data == val) {
            prev->next = new Node(el, tmp);
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    return head;
}
*/
// OPTIMISED
Node* insertBeforeValue(Node* head, int el, int val) {
    if (head == NULL) {
        return head;
    }
    if (head->data == val) return new Node(el, head);

    Node* tmp = head;
    while(tmp->next != NULL) {
        if (tmp->next->data == val) {
            tmp->next = new Node(el, tmp->next);
            break;
        }
        tmp = tmp->next;
    }
    return head;
}

Node* reverseLL(Node *head) {
    Node *newHead = NULL;
    while(head != NULL) {
        Node *next = head->next;
        head->next = newHead;
        newHead = head;
        head = next;
    }
    return newHead;
}


Node* removeElements(Node* head, int val) {
    while (head && head->data == val) {  // Handle head deletions
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    Node* curr = head;
    Node* prev = nullptr;

    while (curr) {
        if (curr->data == val) {
            prev->next = curr->next;
            delete curr;
            curr = prev->next;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    return head;
}

/* Remove duplicates from sorted */
Node* deleteDuplicates(Node* head) {
    Node* current = head;
    Node* headptr = head;
    while (current && current->next) {
        if(current->data == current->next->data){
            current->next = current->next->next;
        }
        else{
            current = current->next;
        }
    }
    return headptr;
}


// Node* reverseKGroup(Node* head, int k) {
    
//     Node* KthNode = head;
//     bool first = true;
//     Node* newHead = head;

//     int cnt = 0;
//     while(cnt == k) {
        
//         while(KthNode && (cnt < k)) {
//             cnt++;
//             KthNode = KthNode->next;
//         }
//         Node* newNext = KthNode->next;
//         KthNode->next = nullptr;
//         Node* headAfter = reverseLL(head);
//         if (first) newHead = headAfter;
//         first = false;
//         head->next = newNext;
//         head = newNext;
//     }
//     return newHead;
// }
Node* reverseKGroup(Node* head, int k) {
    if (!head || k <= 1) return head;  // Edge case handling

    Node* KthNode = head;
    Node* newHead = nullptr;
    Node* prevTail = nullptr;

    while (KthNode) {
        int cnt = 0;
        Node* groupHead = KthNode;
        Node* groupTail = KthNode;

        // Move `KthNode` k nodes ahead
        while (cnt < k && KthNode) {
            groupTail = KthNode;
            KthNode = KthNode->next;
            cnt++;
        }

        // If the group size is less than k, do not reverse
        if (cnt < k) {
            if (prevTail) prevTail->next = groupHead;  // Connect last reversed part
            break;
        }

        // Break the current segment
        groupTail->next = nullptr;

        // Reverse the current k-group
        Node* reversedHead = reverseLL(groupHead);

        // Update the new head for the first group
        if (!newHead) newHead = reversedHead;

        // Connect previous part to reversed group
        if (prevTail) prevTail->next = reversedHead;

        // Move `prevTail` to the last node of reversed segment
        prevTail = groupHead;  // `groupHead` is now the last node after reversing

        // Move `groupHead` to `KthNode` (next segment start)
        groupHead->next = KthNode;
    }

    return newHead ? newHead : head;
}

Node* rotateRight(Node* head, int k) {
    if (!head || k < 1) return head;
    Node* tmp = head;

    while(tmp || k > 0) {
        tmp = tmp->next;
        k--;
    }
    if (k) return head;
    Node* kth = head;
    while(tmp) {
        tmp = tmp->next;
        kth = kth->next;
    }
    Node* newHead = kth->next;
    kth->next = nullptr;
    tmp->next = head;
    return newHead;
}

int main() {

    std::vector<int> arr = {1,2,3,4};
    Node* head = convertArr2LL(arr);
    head = insertHead(head, 100);
    // OR
    head = new Node(99, head);
    printLL(head);

    head = insertTail(head, 100);
    printLL(head);

    head = insertKth(head, 101, 3);
    printLL(head);

    head = insertKth(head, 98, 1);
    printLL(head);

    head = insertKth(head, 101, 10);
    printLL(head);
    head = insertKth(head, 101, 12);
    printLL(head);

    head = insertBeforeValue(head, 5, 2);
    printLL(head);
    head = insertBeforeValue(head, 97, 98);
    printLL(head);
    head = insertBeforeValue(head, 102, 101);
    printLL(head);
    // 97 98 99 100 102 101 1 5 2 3 4 100 101


    /*
    std::vector<int> arr = {1,2,3,4};
    // Node* a = new Node(arr[0], nullptr); // new give pointer to the memory location
    Node a = Node(arr[0], nullptr); // we are creating an object
    std::cout << a.next << std::endl;

    Node* head = convertArr2LL(arr);
    printLL(head);

    std::cout << "Val is " << (checkIfPresent(head, 4) ? "present\n" : "not present\n");

    // Node* afterDelete = deleatLL(head, 1);
    // printLL(afterDelete);

    // Node* afterDelete2 = deleatLL(head, 4);
    // printLL(afterDelete2);

    head = deleteHead(head);
    printLL(head);

    head = deleteK(head, 2);
    printLL(head);

    head = deleteEl(head, 4);
    printLL(head);


    head = insertHead(head, 4);
    printLL(head);
    */
    
}