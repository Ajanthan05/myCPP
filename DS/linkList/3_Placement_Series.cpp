#include <iostream>
#include <vector>
#include <climits>

struct Node
{
public:
    int val;
    Node* next;

    // Constructor with two arguments
    Node(int _val, Node* _next)
    {
        val = _val;
        next = _next;
    }

    // Constructor with one argument
    Node(int _val)
    {
        val = _val;
        next = nullptr;
    }

    // Default constructor
    Node()
    {
        val = 0;
        next = nullptr;
    }
};

// Node* Remove_Nth_Node_From_End(Node *head, int n) {
//     Node *tmp = head;
//     int count = 0;
//     while(tmp  != NULL) {
//         tmp = tmp->next;
//         count++;
//     }
//     n -= count;
//     if (n < count) {

//     } 
// }

Node* Remove_Nth_Node_From_End(Node *head, int n) {
    Node *start = new Node();
    start->next = head;

    Node *fast = start;
    Node *slow = start;

    for (int i=0; i<n; i++) {
        fast = fast->next;
    }

    while(fast->next != NULL) {
        fast = fast->next;
        slow = slow->next;
    }
    slow->next = slow->next->next;
    // return head;

    Node* result = start->next; // Points to the actual head of the modified list
    delete start;               // Clean up the dummy node
    return result;
}

class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode *fast = head, *slow = head;

        // Move fast pointer n steps ahead
        for (int i = 0; i < n; ++i) {
            if (fast == nullptr) return head;  // If n is greater than the length of the list
            fast = fast->next;
        }

        // Special case: if fast reaches the end, the head node needs to be deleted
        if (fast == nullptr) {
            ListNode* temp = head;
            head = head->next;
            delete temp;  // Free the memory of the old head
            return head;
        }

        // Move both pointers until fast reaches the last node
        while (fast->next != nullptr) {
            fast = fast->next;
            slow = slow->next;
        }

        // Slow is now at the node before the one we want to remove
        ListNode* temp = slow->next;
        slow->next = slow->next->next;  // Remove the nth node from the end
        delete temp;  // Free the memory of the removed node

        return head;
    }
};




Node* middleNode(Node *head) {
    Node* fast = head, *slow = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

Node *mergeTwoLists(Node *l1, Node *l2) {
    if (l1 == NULL) return l2;
    if (l2 == NULL) return l1;
    if (l1->val > l2->val) std::swap(l1, l2);

    Node *res = l1;
    while (l1 != NULL && l2 != NULL) {
        Node *tmp = NULL;
        while (l1 != NULL && l1->val <= l2->val) {
            tmp = l1;
            l1 = l1->next;
        }
        tmp->next = l2;
        std::swap(l1, l2)
    }
    return res;
}

void deletNode(Node *node) {
    if (node->next == NULL) {
        node = NULL;
        return;
    }
    node->val = node->next->val;
    Node *tmp = node->next;
    node->next = node->next->next;
    delete tmp;
}

Node *add2Numbers(Node *l1, Node *l2) {
    Node *dummy = new Node();
    Node *tmp = dummy;

    int carry = 0;
    while (l1 != NULL || l2 != NULL || carry) {
        int sum = 0;

        if (l1 != NULL) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2 != NULL) {
            sum += l2->val;
            l2 = l2->next;
        }

        sum += carry;
        carry = sum / 10;
        tmp->next = new Node(sum % 10);
        tmp = tmp->next;
    }
    return dummy->next;
}

Node *intersectionOf2LL(Node *l1, Node *l2) {
    if (l1 == NULL || l2 == NULL) return NULL;

    Node *a = l1;
    Node *b = l2;

    while (a != b) {
        a = (a == NULL? l2 : a->next);
        b = (b == NULL? l1 : b->next);
    }
    return a;
}

bool hasCycle(Node *head) {
    if (head == NULL || head->next == NULL) return false;

    Node *fast = head;
    Node *slow = head;

    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;

        if(fast == slow) return true;
    }
    return false;
}

Node * reverseLL(Node *head) {
    Node *pre = NULL;
    Node *next = NULL;

    while (head != NULL) {
        next = head->next;
        head->next = pre;
        pre = head;
        head = next;
    }
    return pre;
}

bool isPalindrome(Node *head) {
    if (head == NULL || head->next == NULL) return true; // single word is palindrom

    Node *fast = head;
    Node *slow = head;

    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    
    slow->next = reverseLL(slow->next);
    slow = slow->next;

    while (slow  != NULL) {
        if (head->val != slow->val) return false;
        head = head->next;
        slow = slow->next;
    }
    return true;
}

Node *reverseKGroup(Node *head, int k) {
    if (head == NULL || k == 1) return head;

    Node *dummy = new Node();
    dummy->next = head;
    Node *cur = dummy, *pre = dummy, *nex = dummy;
    int count = 0;

    while(cur->next != NULL) {
        cur = cur->next;
        count++;
    }

    while (count >= k) {
        cur = pre->next;
        next = cur->next;

        for (int i=1; i<k; i++) {
            cur->next = nex->next;
            nex->next = pre->next;
            pre->next = nex;
            nex = cur->next;
        }
        pre = cur;
        count -= k;
    }
    return dummy->next;
}

Node *entryPointOfCycle(Node * head) {
    if (head == NULL || head->next == NULL) return NULL;

    Node *slow = head;
    Node *fast = head;
    Node *entry = head;

    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;

        if (fast == slow) {
            while (slow != entry) {
                slow = slow->next;
                entry = entry->next;
            }
            return entry;
        }
    }
    return NULL;
}

Node *rotateK(Node *head, int k) {
    if (!head || !head->next || k < 1) return head;

    Node *cur = head;
    int len = 1;
    while(cur->next && len++) cur = cur->next;

    cur->next = head;
    k = k % len;
    k = len - k;

    while(k--) cur = cur->next;

    head = cur->next;
    cur->next = NULL;

    return head;
}

Node* copyRandomList(Node* head) {
    Node *iter = head; 
    Node *front = head;

    while(iter != NULL) {
        Node* front = iter->next;
        iter->next = new Node(iter->val, front);
        iter = front; //iter->next->next;
    }

    // Second round: assign random pointers for the copy nodes.
    iter = head;
    while(iter != NULL) {
        if (iter->random != NULL) {
            iter->next->random = iter->random->next;
        }
        iter = iter->next->next;
    }

    // 3rd Round
    iter = head;
    Node *dummy = new Node(0);
    Node *copy = dummy;

    while(iter != NULL) {
        front = iter->next->next;
        copy->next = iter->next;
        iter->next = front;

        copy = copy->front;
        iter = front;
    }
    return dummy->next;
}

int main() {


    return 0;
}