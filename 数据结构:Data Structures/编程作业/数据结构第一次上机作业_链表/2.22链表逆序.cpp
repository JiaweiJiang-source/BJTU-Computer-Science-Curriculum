#include <stdio.h>
#include <stdlib.h>

// 定义单链表节点结构
struct ListNode {
    int data;
    struct ListNode* next;
};

// 创建新节点
struct ListNode* createNode(int data) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (newNode) {
        newNode->data = data;
        newNode->next = NULL;
    }
    return newNode;
}

// 就地逆置单链表
void reverseList(struct ListNode** head) {
    struct ListNode* prev = NULL;
    struct ListNode* current = *head;
    struct ListNode* nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next;
        current->next = prev;
        prev = current;
        current = nextNode;
    }

    *head = prev; // 更新头指针
}

// 打印链表
void printList(struct ListNode* head) {
    struct ListNode* current = head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// 释放链表空间
void freeList(struct ListNode* head) {
    struct ListNode* current = head;
    while (current) {
        struct ListNode* temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    struct ListNode* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);

    printf("原始链表：");
    printList(head);

    reverseList(&head);
    printf("逆置后的链表：");
    printList(head);

    freeList(head);

    return 0;
}

