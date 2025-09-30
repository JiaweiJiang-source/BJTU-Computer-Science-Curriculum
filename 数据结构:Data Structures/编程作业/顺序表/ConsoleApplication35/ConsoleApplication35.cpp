#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>

//定义员工结构体
typedef struct
{
	char name[51];
	char position[101];
	int id; 
}Employee;

//定义顺序表结构体
typedef struct
{
	Employee* data;	//存储员工信息
	int length;		//线性表长度
	int MaxSize;	//顺序表容量
}SeqList;

//初始化一个顺序表--动态分配内存实现
void InitList(SeqList &L,int MaxSize)
{
	//使用malloc动态分配内存
	L.data = (Employee*)malloc(sizeof(Employee) * 2000);
	L.length = 0;
	L.MaxSize = MaxSize;
}

//在位置i中插入信息
bool InsertList(SeqList& L, int i, Employee employee)
{
	if (i<0 || i>L.length)
		return false;

	for (int j = L.length; j > i; j--)
		L.data[j] = L.data[j - 1];

	L.data[i] = employee;
	L.length++;
	return true;
}

//删除x处的员工信息
bool DeleteList(SeqList& L, int x)
{
	//将x位置之后往前移动
	for (int i = x - 1; i < L.length - 1; i++)
		L.data[i] = L.data[i + 1];
	L.length--;
	return true;
}

//查找x处的员工信息
bool search(SeqList& L, int x)
{
	printf("%s %s %d\n", L.data[x - 1].name, L.data[x - 1].position, L.data[x - 1].id);
	return true;
}

//打印员工信息
void Print(SeqList& L)
{
	for (int i = 0; i < L.length-1; i++)
		printf("%s %s %d ", L.data[i].name, L.data[i].position, L.data[i].id);//为何直接将其放入循环 i = L.length - 1会无限打印L.data[i].position
	printf("%s %s %d ", L.data[L.length - 1].name, L.data[L.length -1].position, L.data[L.length -1].id);
	printf("\n");
}

//销毁链表
void DestroySeqList(SeqList &L) {
	free(L.data);
	L.data = NULL;
	L.length = 0;
	L.MaxSize = 0;
}

int main(void)
{
	int n, m;	//n表示员工个数，m表示操作次数
	scanf("%d %d", &n, &m);

	//初始化链表
	SeqList L;
	InitList(L, n);

	//读取信息，加入顺序表
	for (int i = 0; i < n; i++)
	{
		Employee employee;
		scanf("%s %s %d", employee.name, employee.position, &employee.id);
		InsertList(L, i, employee);
	}


	for (int i = 0; i < m; i++)
	{
		int op, x;
		char name_1[500], position_1[500];
		int id_1;

		scanf("%d %d", &op, &x);

		//printf("len = %d\n", L.length);
		if (op == 1)	//在x处员工离职
		{
			if (x <= 0 || x > L.length)
				printf("-1\n");
			else
			{
				DeleteList(L, x);
				Print(L);
			}
		}
		else if (op == 2)//入职
		{
			if (x <= 0 || x > L.length + 1)
			{
				
				scanf("%s %s %d", name_1, position_1, &id_1);
				printf("-1\n");
			}
			else
			{
				Employee employee;
				scanf("%s %s %d", employee.name, employee.position, &employee.id);
				InsertList(L, x - 1, employee);
				Print(L);
			}
		}
		else if (op == 3)//查询
		{
			//printf("x = %d\n", x);
			if (x <= 0 || x > L.length)
				printf("-1\n");
			else
			{
				search(L, x);
			}
		}
	}
	DestroySeqList(L);
}