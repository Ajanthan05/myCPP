#include<iostream>
// #include<type_traits>
#include<string>
// #include<tuple>

// int ATOI(char *str, int n) {
//     // int n = str.length();
//     int ans;
//     for(int i=n-1; i>=0; --i) {
//     // while()
//         // int tmp = str[i] - '0';
//         ans = ans*10 + str[i] - '0';
//     }
//     return ans;
// }

struct Node {
public:
    int data;
    Node *next;

    Node(int val) {
        data = val;
        next = nullptr;
    }

    Node(int val, Node *nex) {
        data = val;
        next = nex;
    }
};

void Swap(Node *head, int ind) {
    ind = ind-1;
    while(ind-- && head && head->next) {
        head = head->next;
    }

    Node *prev = head;
    Node *Cur = head->next;
    Node *Next = head->next->next;

    prev->next = Next;
    Next->next = Cur;

}

void Print(Node *head) {
    while(head) {
        std::cout << head->data << " ";
        if (head->next) head = head->next;
    }
    std::cout << "\n";
}

int main(){
    // char* c = "123";
    // int n = sizeof(c)/sizeof(char);
    // printf("%d", n);
    // printf("Val: %d\n", ATOI(c, n));

    Node *head = new Node(1);
    
    Node *tmp = head;
    for(int i=2; i<=5; ++i) {
        Node *t2 = new Node(i);
        tmp->next = t2;
        tmp = tmp->next;
    }
    Print(head);
    // Swap(head, 3);
    Print(head);
}