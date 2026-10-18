#include <iostream>
#include<stdio.h>

struct Node {
    int data;
    Node* next;
};

void pushBack(Node*& head, int value) {
    Node* node = new Node{value, nullptr};

    if (head == nullptr) {
        head = node;
        return;
    }

    Node* cur = head;
    while (cur->next != nullptr) {
        cur = cur->next;
    }
    cur->next = node;
}

void printList(const Node* head) {
    const Node* cur = head;
    while (cur != nullptr) {
        std::cout << cur->data << " -> ";
        cur = cur->next;
    }
    std::cout << "nullptr\n";
}

Node* reverseListRecursive(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    Node* newHead = reverseListRecursive(head->next);
    head->next->next = head;
    head->next = nullptr;

    return newHead;
}

int findNthIndex(const Node* head, int value, int n) {
    if (n <= 0) return -1;

    int index = 0;
    int count = 0;
    const Node* cur = head;

    while (cur != nullptr) {
        if (cur->data == value) {
            ++count;
            if (count == n) {
                return index;
            }
        }
        cur = cur->next;
        ++index;
    }
    return -1;
}

int main() {
    Node* head = nullptr;
    pushBack(head, 1);
    pushBack(head, 2);
    pushBack(head, 5);
    pushBack(head, 3);
    pushBack(head, 5);
    pushBack(head, 4);
    printList(head);
    head = reverseListRecursive(head);
    printf("%i\n",findNthIndex(head, 5, 1));
    printf("%i\n",findNthIndex(head, 5, 2));

    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}