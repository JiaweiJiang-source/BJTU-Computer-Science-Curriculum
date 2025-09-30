#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<algorithm>

using namespace std;
//ʱ�临�Ӷ�ΪO(n) 
typedef struct LNode
{
	int data;
	struct LNode* next;
}LNode,*LinkList;

//�����½ڵ�
LNode* createNode(int a)
{
	LNode* newNode = (LNode*)malloc(sizeof(LNode));
	if (newNode)
	{
		newNode->data = a;
		newNode->next = NULL;
	}
	return newNode;
}

//ɾ�����
void DeleteNode(LinkList& L, int mink, int maxk)
{
	LNode* p = L->next;
	LNode* s = NULL;

	while (p)
	{
		if (p->data > mink && p->data < maxk)
		{
			s->next = p->next;
			free(p);
			p = s->next;
		}
		s = p;
		p = p->next;
	}
}

//��ӡ����
void printList(LinkList& L)
{
	LNode* p = L;
	while (p)
	{
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
}

// �ͷ�����ռ�
void freeList(LinkList& L) {
	LNode* current = L;
	while (current) {
		LNode* temp = current;
		current = current->next;
		free(temp);
	}
}

int main(void)
{
	int a, n;
	int mink, maxk;

	LinkList L;
	L = createNode(0);
	L->next = createNode(1);
	L->next->next = createNode(2);
	L->next->next->next = createNode(3);
	L->next->next->next->next = createNode(4);

	printf("ԭʼ����");
	printList(L);

	printf("������mink��maxkֵ��\n");
	scanf("%d %d", &mink, &maxk);

	DeleteNode(L, mink, maxk);
	printf("�޸ĺ�����");
	printList(L);
	freeList(L);

	return 0;
}

