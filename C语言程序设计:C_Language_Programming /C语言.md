# C语言

## 素数

写函数输出从 m 到 n 之间的素数（1<m<n<10000）；函数原型如下：



```c
void primes(int m, int n)
{
    for(int i = m;i<n;i++)
    {
        int flag = 1;
       for(int j = 2;j<m;j++)
       {
           if(i%j==0)
           {
               flag = 0;
               break;
           } 
       }
       if(flag)
           printf("%d",i);
    }
}
```

## Fibonacci递归

```c
long fibo( int n)
{   
    long m;
    if ( n==1) m=1;
    else if( n==2 ) m=1;
    else m=fibo( n-2 )+fibo( n-1 );
   return m;
}
```

## 求两个数最大公约数的递归函数

```c
int GCD(int m, int n)  // 定义一个函数 GCD，接受两个整数参数 m 和 n
{
    int gcd;  // 声明一个整数变量 gcd，用于存储最大公约数

    if (m % n == 0)  // 检查 m 是否能被 n 整除
        gcd = n;  // 如果能整除，则 n 就是最大公约数，将其赋值给 gcd
    else
        gcd = GCD(n, m % n);  // 如果不能整除，递归调用 GCD 函数，参数为 n 和 m 除以 n 的余数
    return gcd;  // 返回计算得到的最大公约数
}
```

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250102214633276.png" alt="image-20250102214633276" style="zoom:33%;" />

```c
int gcd(int m, int n)
{
    int gcd;
    if(m%n==0)
        gcd = n;
    else
   		gcd(n, m%n);
    return gcd;
}
```



## 汉诺塔移动的递归函数

```c
void move(int n, int from, int assit, int to)
{
    if(n==1)
        printf("将第1块从%d移动到%d", from, to);
    else
    {
        move(n - 1, from, to, assit);
        printf("将第%d号金盘从%c移动到%c \n",from, assit);
        move(n - 1, assit, from, to);
    }
}
```

## 冒泡法

```c
for (i = 1; i < n; i++)  // 外层循环，控制排序的轮数，共进行 n-1 轮
{
    for (j = 0; j < n - i; j++)  // 内层循环，控制每轮比较的次数，每轮比较 n-i 次
    {
        if (a[j] > a[j + 1])  // 比较相邻的两个元素，如果前一个元素大于后一个元素
        {
            med = a[j];        // 使用临时变量 med 保存 a[j] 的值
            a[j] = a[j + 1];   // 将 a[j+1] 的值赋给 a[j]
            a[j + 1] = med;    // 将 med 的值赋给 a[j+1]，完成交换
        }
    }
}
```

```c
for(int i = 0; i < n - 1; i++)
{
    for(int j = 0; j< n - i - 1; j++)
    {
        if(a[j] > a[j+1])
        {
            int temp = a[j];
            a[j] = a[j+1];
            a[j+1] = temp;
        }
    }
}
```

## 选择排序算法

```c
for (i = 0; i < n - 1; i++) {
    p = i;
    // 每次选择最大的数字放到最前面
    for (j = i + 1; j < n; j++)
        if (a[j] > a[p]) 
            p = j;
    if (p != i) {
        med = a[i];
        a[i] = a[p];
        a[p] = med;
    }
}
```

```c
for(int i = 0; i<n;i++)
{
    int temp = i;
    for(int j = i+1;j<n;j++)
    {
        if(a[j]>a[temp])
           temp = j; 
    }
    if(i!=temp)
    {
        int mid = a[i];
        a[i] = a[temp];
        a[temp] = mid;
    }
}
```

## 折半查找

```c
#include <stdio.h>
#define N 15
// 前提：已经顺序排好
void main() {
    int a[100], key, i, low, high, mid, find = 0;
    printf("请输入数据:\n");
    for (i = 1; i < N; i++) 
        scanf("%d", &a[i]);
    printf("输入要查找的数据 key:\n");
    scanf("%d", &key);
    low = 1;
    high = N - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (key == a[mid]) {
            find = 1;
            break;
        } 
        else if (key < a[mid]) 
        {
            high = mid - 1;
        } 
        else 
        {
            low = mid + 1;
        }
    }
    if (find) 
        printf("%d\n", mid);
    else 
        printf("%d\n", mid = 0);
}
```

```c
void find(int key, int a[], int high, int low)
{
    int find = 0;
    
    while(low<=high)
    {
        mid = (low+high)/2;
        if(key == find)
        {
            find = 1;
            break;
        }
        else if(key<a[mid])
        	high = mid - 1;
        else
            low = mid + 1;
    }
    if(find)
        printf("find");
    else
        printf("not found！\n");
}
```



## 字符数组初始化

```c
char str[20]="good morning!";	// 初始化
char *psrt = "good morning!";	// 初始化
char *pstr;
pstr = "good morning!";			// 赋值
```

## 指针变量初始化

```c
int a;
int *p = &a;

int *p;
p = &a;
// 指针变量不允许赋值为数字
// 错误:
int *p;
p = 1000;
// 不能直接赋值： *p = &a
```

## 指针与函数

```c
// 返回指针值的函数
int  *search(int *x，int n)
{    
    int *p；
	p = x+n；
	return(p)；	
}

// 函数的指针：定义了一个指向函数的指针变量p，函数的返回值是整型
// (*指针变量)(实参表列)；      
int (*p)();	// 注意和int *p() 的区别


int square(int x) {
    return x * x;
}

int main() {
    int (*p)(int);  // 声明一个函数指针 p
    p = square;     // 将 square 函数的地址赋值给 p
    printf("%d\n", p(5));  // 通过 p 调用 square 函数，输出 25
    return 0;
}
```

## 指针数组

```c
int *a[4];	//等同于 int *(a[4])； 
/*
定义了一个有4个元素的一维数组；
数组的每个分量都是一个指向整型的指针；
*/
int (*a)[4];
//定义了一个行指针变量 a，指向有 4 个元素的一维数组。
```

## 动态分布

```c
#include<malloc.h>
int *p;
p = (int*)malloc(sizeof(int));
p = (int *)calloc( 10 , sizeof(int));
free(p);
```

## 链表

```c
struct student
{
    int num;
    float score;
    struct student *next;
}LNode, *LinkList;

void InitList(LinkList L)
{
    L = (LNode*)malloc(sizeof(LNode));
    if(!L)
        exit(0);
    L->next = L;
}
```

## 文件

```c
FILE *fp;
if(fp = fopen("loveyou.txt","r") == NULL)
{
    return error;
}
...
fclose(fp);
```

## Attention

二维数组初始化只可以行省略$$a[][3]$$

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250103113315711.png" alt="image-20250103113315711" style="zoom:25%;" />

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250103115540913.png" alt="image-20250103115540913" style="zoom:50%;" />

<img src="C:\Users\37623\AppData\Roaming\Typora\typora-user-images\image-20250103115603731.png" alt="image-20250103115603731" style="zoom:50%;" />

rewind 函数回到初始位置



## 链表

```c
typedef struct student｛
 int num；
 char name[20]；
 struct student *next；
}STU;
```

### 创建链表

```c
void create_link(STU **phead)
{
    *phead = (STU*)malloc(sizeof(STU));
    (*phead)->next = NULL;
}
```

