#include <stdio.h>
#include <stdlib.h>

// 定义双向循环链表节点结构
struct ListNode {
    int data;
    int freq;
    struct ListNode* prior;
    struct ListNode* next;
};

// 创建新节点
struct ListNode* createNode(int data) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (newNode) {
        newNode->data = data;
        newNode->freq = 0;
        newNode->prior = NULL;
        newNode->next = NULL;
    }
    return newNode;
}

// 将节点插入到双向循环链表中
void insertNode(struct ListNode** head, struct ListNode* newNode) {
    // 空链表
    if (*head == NULL) {
        *head = newNode;
        (*head)->prior = *head;
        (*head)->next = *head;
    }
    else {
        // 将新节点插入到头节点之后
        newNode->next = (*head)->next;
        (*head)->next->prior = newNode;
        newNode->prior = *head;
        (*head)->next = newNode;
    }
}

// 交换节点的位置
void swapNodes(struct ListNode* node1, struct ListNode* node2) {
    int temp_data = node1->data;
    int temp_freq = node1->freq;

    node1->data = node2->data;
    node1->freq = node2->freq;

    node2->data = temp_data;
    node2->freq = temp_freq;
}

// 定位节点，并根据频度调整链表结构
struct ListNode* locate(struct ListNode** head, int x) {
    struct ListNode* current = (*head)->next;

    // 遍历链表，查找节点
    while (current != *head) {
        if (current->data == x) {
            current->freq++; // 频度加1

            // 若当前节点的频度比前一个节点的频度大，则交换位置
            while (current != *head && current->freq > current->prior->freq) {
                swapNodes(current, current->prior);
                current = current->prior;
            }

            return current;
        }

        current = current->next;
    }

    // 节点不存在，返回NULL
    return NULL;
}

// 打印链表
void printList(struct ListNode* head) {
    struct ListNode* current = head->next;
    while (current != head) {
        printf("%d(%d) ", current->data, current->freq);
        current = current->next;
    }
    printf("\n");
}

// 释放链表空间
void freeList(struct ListNode* head) {
    struct ListNode* current = head->next;
    while (current != head) {
        struct ListNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(head);
}

int main() {
    struct ListNode* head = createNode(0); // 头节点

    // 插入节点
    insertNode(&head, createNode(1));
    insertNode(&head, createNode(2));
    insertNode(&head, createNode(3));

    printf("初始链表：");
    printList(head);

    // 进行locate操作
    struct ListNode* node = locate(&head, 2);
    if (node != NULL) {
        printf("定位到节点：%d(%d)\n", node->data, node->freq);
    }
    else {
        printf("节点不存在\n");
    }

    printf("调整后的链表：");
    printList(head);

    freeList(head);

    return 0;
}

