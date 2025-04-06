#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

bool hasCycle(ListNode *head) {
    ListNode* tmp = head;

    unordered_map<ListNode*, int> mpp;
    while(tmp) {
        if (mpp.find(tmp) != mpp.end()) return true;
        mpp.insert({tmp, 1});
        tmp = tmp->next;
    }
    return false;
}

/* TOTISE AND HAIR ALGORITHM */
bool hasCycle(ListNode *head) {
    if (!head || !head->next) return false; // Handle empty or single-node case

    ListNode* slow = head;
    ListNode* fast = head; // Start both at head

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true; // Cycle detected
    }
    return false; // No cycle found
}

/* Find the length of the Loop in LinkedList 
1) hashing map + timer

2>
*/
int lengthLoop(ListNode *head) {
    if (!head || !head->next) return 0;

    ListNode* slow = head;
    ListNode* fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            int cnt = 0;
            do {
                slow = slow->next;
                cnt++;
            } while(slow != fast); // Fix condition to correctly count cycle length
            return cnt;
        }
    }
    return 0;
}

/*   Delete the middle node of the LinkedList */
ListNode* deleteMiddle(ListNode* head) {
    if (!head || !head->next) return nullptr; // Edge case: single node

    ListNode* slow = head;
    ListNode* fast = head;
    ListNode* prev = nullptr;

    while (fast && fast->next) {
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }
    prev->next = slow->next;
    delete slow;

    return head;
}

/* Skip slow by 1 step */
ListNode* deleteMiddle(ListNode* head) {
    if (!head || !head->next) return nullptr; // Edge case: single node

    ListNode* slow = head;
    ListNode* fast = head;
    fast = fast->next->next;

    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    ListNode* middle = slow->next;
    slow->next = slow->next->next;
    delete middle;

    return head;
}


/* Find the starting point of the Loop/Cycle in LinkedList  */
ListNode *detectCycle(ListNode *head) {
    if (!head || !head->next) return nullptr;
    
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;

        if (fast == slow) {
            ListNode* tmp = head;
            while(tmp != slow) {
                tmp = tmp->next;
                slow = slow->next;
            }
            return tmp;
        }
    }
    return nullptr;
}

/* Optimal 
1. Early Exit When No Cycle is Found
Your New Code:

cpp
Copy
Edit
if (fast == nullptr || fast->next == nullptr)
    return nullptr;
Why it's faster?

As soon as fast reaches nullptr, it immediately exits and avoids unnecessary iterations.

Previous Code:

cpp
Copy
Edit
while (fast && fast->next) { ... }
return nullptr;
Why it's slightly slower?

The previous version loops through unnecessary iterations when there is no cycle.

2. Directly Using head Instead of an Extra Variable
Your New Code:

cpp
Copy
Edit
while (head != slow) {
    head = head->next;
    slow = slow->next;
}
return slow;
Why it's faster?

It directly moves head instead of introducing an extra variable (tmp), reducing memory accesses.

Previous Code:

cpp
Copy
Edit
ListNode* tmp = head;
while (tmp != slow) {
    tmp = tmp->next;
    slow = slow->next;
}
return tmp;
Why it's slower?

Uses an extra pointer (tmp) which adds a minor overhead.

3. Better Branch Prediction & Cache Efficiency
The early exit condition (fast == nullptr || fast->next == nullptr) improves branch prediction, helping the CPU avoid unnecessary jumps.

By using fewer variables and fewer redundant conditions, the code is more cache-efficient, reducing CPU cycles.

Performance Comparison:
While both implementations are O(N) time complexity, your new version is slightly faster in practice due to: ✅ Fewer conditional checks
✅ Faster early termination
✅ Better cache usage
✅ Fewer memory accesses

It may not always be noticeable for small lists but provides a minor boost in large-scale linked lists.
*/
ListNode *detectCycle(ListNode *head) {
    if(head==nullptr) return head;
    ListNode* slow=head;
    ListNode* fast=head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;       // Move slow one step
        fast = fast->next->next; // Move fast two steps
        
        if (slow == fast) {      // Cycle detected
            break;
        }
    }
    if(fast==nullptr || fast->next==nullptr)
        return nullptr;
    while(head!=slow)
    {
        head=head->next;
        slow=slow->next;
    }
    return slow;
}


int main() {


    return 0;
}