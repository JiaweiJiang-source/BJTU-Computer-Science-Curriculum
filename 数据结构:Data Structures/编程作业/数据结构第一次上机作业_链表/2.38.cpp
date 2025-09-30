#include <stdio.h>
#include <stdlib.h>

// ����˫��ѭ������ڵ�ṹ
struct ListNode {
    int data;
    int freq;
    struct ListNode* prior;
    struct ListNode* next;
};

// �����½ڵ�
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

// ���ڵ���뵽˫��ѭ��������
void insertNode(struct ListNode** head, struct ListNode* newNode) {
    // ������
    if (*head == NULL) {
        *head = newNode;
        (*head)->prior = *head;
        (*head)->next = *head;
    }
    else {
        // ���½ڵ���뵽ͷ�ڵ�֮��
        newNode->next = (*head)->next;
        (*head)->next->prior = newNode;
        newNode->prior = *head;
        (*head)->next = newNode;
    }
}

// �����ڵ��λ��
void swapNodes(struct ListNode* node1, struct ListNode* node2) {
    int temp_data = node1->data;
    int temp_freq = node1->freq;

    node1->data = node2->data;
    node1->freq = node2->freq;

    node2->data = temp_data;
    node2->freq = temp_freq;
}

// ��λ�ڵ㣬������Ƶ�ȵ�������ṹ
struct ListNode* locate(struct ListNode** head, int x) {
    struct ListNode* current = (*head)->next;

    // �����������ҽڵ�
    while (current != *head) {
        if (current->data == x) {
            current->freq++; // Ƶ�ȼ�1

            // ����ǰ�ڵ��Ƶ�ȱ�ǰһ���ڵ��Ƶ�ȴ��򽻻�λ��
            while (current != *head && current->freq > current->prior->freq) {
                swapNodes(current, current->prior);
                current = current->prior;
            }

            return current;
        }

        current = current->next;
    }

    // �ڵ㲻���ڣ�����NULL
    return NULL;
}

// ��ӡ����
void printList(struct ListNode* head) {
    struct ListNode* current = head->next;
    while (current != head) {
        printf("%d(%d) ", current->data, current->freq);
        current = current->next;
    }
    printf("\n");
}

// �ͷ�����ռ�
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
    struct ListNode* head = createNode(0); // ͷ�ڵ�

    // ����ڵ�
    insertNode(&head, createNode(1));
    insertNode(&head, createNode(2));
    insertNode(&head, createNode(3));

    printf("��ʼ����");
    printList(head);

    // ����locate����
    struct ListNode* node = locate(&head, 2);
    if (node != NULL) {
        printf("��λ���ڵ㣺%d(%d)\n", node->data, node->freq);
    }
    else {
        printf("�ڵ㲻����\n");
    }

    printf("�����������");
    printList(head);

    freeList(head);

    return 0;
}

