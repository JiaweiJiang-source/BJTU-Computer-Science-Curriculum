#include <stdio.h>
#include <stdlib.h>

// ���嵥����ڵ�ṹ
struct ListNode {
    int data;
    struct ListNode* next;
};

// �����½ڵ�
struct ListNode* createNode(int data) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (newNode) {
        newNode->data = data;
        newNode->next = NULL;
    }
    return newNode;
}

// �͵����õ�����
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

    *head = prev; // ����ͷָ��
}

// ��ӡ����
void printList(struct ListNode* head) {
    struct ListNode* current = head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// �ͷ�����ռ�
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

    printf("ԭʼ����");
    printList(head);

    reverseList(&head);
    printf("���ú������");
    printList(head);

    freeList(head);

    return 0;
}

