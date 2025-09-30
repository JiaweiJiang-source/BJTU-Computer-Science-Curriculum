<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20230427220341113.png" alt="image-20230427220341113" style="zoom:50%;" />





<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20230427220354445.png" alt="image-20230427220354445" style="zoom:50%;" />

```c
第一次用自己完全使用map去解题目，对map中的key和value有了一个更深的理解
unordered_map<key, value>		//是用key去找value
duck_names.insert({ str, i });	//用insert去插入key和value
判断是否是duck_names里面的元素duck_names.count(str)
duck_names[str]通过key去访问value
cnt[duck_names[str]]++;运用了桶的思想
```

```c++
#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<iostream>
#include<algorithm>
#include<unordered_map>
using namespace std;

const int N = 1e5 + 10;
int cnt[N];
int cnt0[N];

int main(void)
{
	int a, b, t,Max1,Max2;
	char str[100];
	unordered_map<string, int>duck_names;
	unordered_map<string, int>goose_names;

	scanf("%d %d", &a, &b);

	for (int i = 0; i < a; i++)
	{
		scanf("%s", str);
		duck_names.insert({ str, i });
	}

	for (int i = a; i < b+a; i++)
	{
		scanf("%s", str);
		goose_names.insert({ str,i });
	}

	for (int i = 0; i < b; i++)
	{
		scanf("%s", str);

		if (duck_names.count(str))
		{
			int value = duck_names[str];
			cnt[value]++;
		}
		else if (goose_names.count(str))
		{
			int value = goose_names[str];
			cnt[value]++;
		}
		else//如果是跳过的
		{
			cnt[a + b]++;
		}
	}

	Max1 = cnt[0];
	Max2 = cnt[1];

	for (int i = 0; i <= a + b; i++)
	{
		if (cnt[i] > Max1)
			Max1 = cnt[i];
		else if (cnt[i] > Max2)
			Max2 = cnt[i];
	}

	scanf("%d", &t);

	for (int i = 0; i < t; i++)
	{
		scanf("%s", str);

		if (duck_names.count(str))	//如果是鸭子阵营，则需要保护
		{
			if (cnt[duck_names[str]] <= Max1 + a)
				printf("Yes\n");
			else
				printf("No\n");
		}
		else if (goose_names.count(str))	//鹅的阵营，全票出
		{
			if (cnt[goose_names[str]] + a <= Max1)
				printf("No\n");
			else if (cnt[goose_names[str]] + a > Max1)
				printf("Yes\n");
		}
		else //跳过票数，判断是否少于Max1且最高票没有平票
		{
			if (cnt[a + b + 1] + a > Max1)
				printf("Yes\n");
			else if (cnt[a + b + 1] + a == Max1)
				printf("Yes\n");
			else if (Max1 == Max2)
				printf("Yes\n");
			else
				printf("No\n");
		}
	}
}
```

