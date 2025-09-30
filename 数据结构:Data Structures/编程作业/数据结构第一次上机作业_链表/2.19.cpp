#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<algorithm>

using namespace std;
//时间复杂度为O(n) 
typedef struct LNode
{
	int data;
	struct LNode* next;
}LNode,*LinkList;

//创建新节点
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

//删除结点
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

//打印链表
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

// 释放链表空间
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

	printf("原始链表：");
	printList(L);

	printf("请输入mink，maxk值：\n");
	scanf("%d %d", &mink, &maxk);

	DeleteNode(L, mink, maxk);
	printf("修改后链表：");
	printList(L);
	freeList(L);

	return 0;
}

