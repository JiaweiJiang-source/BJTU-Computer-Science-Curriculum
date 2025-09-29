#include "windows.h"
#include <conio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include<iostream>
#include<time.h>
using namespace std;
#define MAX 64


void FIFO();          //先进先出算法
void OPT();          //最佳置换算法
void LRU();           //最近最久未使用算法
void CLOCK_pro(int choose);        //改进型Clock置换算法
void Init();    //初始化相关数据结构
void getRandomList(int m, int e, int s);      //随机生成访问序列
void showState();   //显示当前状态及缺页情况
int  isExist(int page);      //查找页面是否在内存
bool randBool(); // 随机生成0或1

struct PageInfo       //页面信息结构
 {
    int  pages[MAX];  // 模拟的最大访问页面数
    int  isVisited;         // 标志位，0表示无页面访问数据
    int  page_missing_num;    // 缺页中断次数
    int  allocated_page_num;     // 分配的页框数
    int  visit_list_length;     // 访问页面序列长度
} pInfo;

struct MemInfo  // 页表项信息
{
    int time; //记录页框中数据的访问历史
    int isVisit;//访问位
    int isModify;//修改位
    int pages;//页号,16位信息，前6位代表页号，后10位为偏移地址 0~2^16
}mInfo;

MemInfo pageList[MAX];      // 分配的页框

int  page_loss_num = 0;                 // 缺页次数
int  current_page;             //页面访问指针
int  replace_page;             //页面替换指针
int  is_loss_page;            //缺页标志，0为不缺页，1为缺页


//初始化相关数据结构
void Init()
{
    int i, pn;
    is_loss_page = 0;              //缺页标志，0为不缺页，1为缺页
    pInfo.page_missing_num = 0;   // 缺页次数
    pInfo.isVisited = 0;        // 标志位，0表示无页面访问数据
    printf("请输入分配的页框数：");   // 自定义分配的页框数
    scanf("%d", &pn);
    pInfo.allocated_page_num = pn;
    for (i = 0; i < MAX; i++)   // 清空页面序列
    {
            pInfo.pages[i] = -1;
    }
}


// 随机生成访问序列
void getRandomList(int m, int e, int s)
{
    int pl;
    Init();     //初始化
    printf("请输入访问序列的长度：");
    scanf("%d", &pl); //随机生成访问序列的长度
    pInfo.visit_list_length = pl;
    srand((unsigned)time(NULL)); // 随机种子
    int  p = rand() % MAX; //在0——MAX范围内随机初始化工作集的起始位置p
    int i, j=0;
    double t= rand() % 10 / 10.0; // 一个范围在0和1之间的值t
    for (i = 0; i < s; i++) // 重复s次
    {
        if (j > pInfo.visit_list_length) // 不能超过访问序列的长度
            break;
        for (j = i * m; j < (i + 1) * m; j++)//生成m个取值范围在p~p + e间的随机数
        {
            pInfo.pages[j]= (p + (rand() % e)) % MAX; //并记录到页面访问序列串中；
        }
        double r = (rand() % 10) / 10.0;// 生成一个范围在0-1之间的随机数r
        if (r < t) // 如果r < t，则为p生成一个新值
            p = rand() % MAX;
        else
        {
            p = (p + 1) % MAX; //否则向后循环移位
        }
    }
}


//  显示当前状态及缺页情况
void showState(void)
{
    int i, n;
    if (current_page == 0)
    {
        printf("\n~~~~~~~~~~~~~~~~~页面访问序列~~~~~~~~~~~~~~~~~\n");
        for (i = 0; i < pInfo.visit_list_length; i++)
        {
            printf("%4d", pInfo.pages[i]);
            if ((i + 1) % 10 == 0) printf("\n");   //每行显示10个
        }
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }
    printf("访问%3d -->内存空间[", pInfo.pages[current_page]);
    for (n = 0; n < pInfo.allocated_page_num; n++)     // 页框信息
        {
        if (pageList[n].pages >= 0)
            printf("%3d", pageList[n].pages);
        else
            printf("   ");
        }
    printf(" ]");
    if (is_loss_page == 1)     //缺页标志，0为不缺页，1为缺页
        {
            printf(" --> 缺页中断 --> ");
            if (current_page == 0)
            {
                printf("置换率 = %3.1f ", (float)(pInfo.page_missing_num) * 100.00 );
            }
            else{
                printf("置换率 = %3.1f", (float)(pInfo.page_missing_num) * 100.00 / current_page);
            }
        }
    printf("\n");
}


// 查找页面是否在内存，1为在内存，0为不在即缺页
int isExist(int page)
{
    int n;
    for (n = 0; n < pInfo.allocated_page_num; n++)
    {
        pageList[n].time++;   // 访问历史加1
    }
    for (n = 0; n < pInfo.allocated_page_num; n++)
    {
        if (pageList[n].pages == page)
        {
            is_loss_page = 0;    //is_loss_page缺页标志，0为不缺页，1为缺页
            pageList[n].time = 0;   //置访问历史为0
            if (randBool()) {
                pageList[n].isVisit = 1;
                pageList[n].isModify = 1;
            }
            else {
                pageList[n].isVisit = 1;
            }
            return 1;
        }
    }
    is_loss_page = 1;  	//页面不存在，缺页
    return 0;
}


//  FIFO页面置换算法
void FIFO(void)
{
    int n, full, status;
    replace_page = 0;   // 页面替换指针初始化为0
    page_loss_num = 0;  // 缺页数初始化为0
    full = 0;           // 是否装满是所有的页框
    for (n = 0; n < pInfo.allocated_page_num; n++) // 清除页框信息
        pageList[n].pages = -1;
    is_loss_page = 0;   //缺页标志，0为不缺页，1为缺页
    for (current_page = 0; current_page < pInfo.visit_list_length; current_page++)  // 执行算法
        {
        status = isExist(pInfo.pages[current_page]);  //查找页面是否在内存
        if (full < pInfo.allocated_page_num)    // 开始时不计算缺页
            {
            if (status == 0)    // 页不存在则装入页面
                {
                pageList[replace_page].pages = pInfo.pages[current_page];
                replace_page = (replace_page + 1) % pInfo.allocated_page_num;
                full++;
                }
            }
        else      // 正常缺页置换
        {
            if (status == 0)    // 页不存在则置换页面
                {
                    pageList[replace_page].pages = pInfo.pages[current_page];
                    replace_page = (replace_page + 1) % pInfo.allocated_page_num;
                    pInfo.page_missing_num++;     // 缺页次数加1
                }
        }
        Sleep(10);
            showState();       // 显示当前状态
        }	 // 置换算法循环结束
        _getch();
    return;
}

// OPT 页面置换算法
void OPT()
{
    int n, full, status;
    replace_page = 0;          // 页面替换指针初始化为0
    page_loss_num = 0;  // 缺页数初始化为0

    full = 0;           // 是否装满是所有的页框
    for (n = 0; n < pInfo.allocated_page_num; n++) // 清除页框信息
    {
        pageList[n].pages = -1;
    }
    is_loss_page = 0;   //缺页标志，0为不缺页，1为缺页
    for (current_page = 0; current_page < pInfo.visit_list_length; current_page++)  // 执行算法
        {
        status = isExist(pInfo.pages[current_page]);  //查找页面是否在内存
        if (full < pInfo.allocated_page_num)    // 开始时不计算缺页
        {
            if (status == 0)    // 页不存在则装入页面
            {
                pageList[replace_page].pages = pInfo.pages[current_page];
                replace_page = (replace_page + 1) % pInfo.allocated_page_num;
                full++;
            }
        }
        else      // 正常缺页置换
        {
            if (status == 0)    // 页不存在,则置换页面
            {
                int min,max = 0 ; //很大的数
                for (int m = 0; m < pInfo.allocated_page_num ; m++)
                {
                    min = 1000;
                    for (int n = current_page; n < pInfo.visit_list_length; n++)
                    {
                        if (pInfo.pages[n] == pageList[m].pages)
                        {
                            min = n;
                            break;
                        }
                    }
                    if (max < min)
                    {
                        max = min;
                        replace_page = m;
                    }

                }
                pageList[replace_page].pages = pInfo.pages[current_page];
                replace_page = (replace_page + 1) % pInfo.allocated_page_num;
                pInfo.page_missing_num++;     // 缺页次数加1
            }
        }
        Sleep(10);
            showState();       // 显示当前状态
        }	 // 置换算法循环结束
        _getch();
    return;
}



//查找替换页面算法，递归实现
int replace_page_pro(int num) {
    int j;
    // 将所有可能被置换的页面排成一个循环队列,用（访问位，修改位）的形式表示各页面状态
    // 1.从当前位置开始扫描到第一个(0,0)的帧用于替换。本轮扫描不修改任何标志位
    for (j = 0; j < pInfo.allocated_page_num; j++){
        if (pageList[(j + num) % pInfo.allocated_page_num].isVisit == 0 && pageList[(j + num) % pInfo.allocated_page_num].isModify == 0)
            return (j+num) % pInfo.allocated_page_num;
    }
    //2. 若第一轮扫描失败，则重新扫描，查找第一个(0,1)的帧用于替换。本轮将所有扫描过的帧访问位设为0
    for (j = 0; j < pInfo.allocated_page_num; j++){
        if (pageList[(j + num) % pInfo.allocated_page_num].isVisit == 0 && pageList[(j + num) % pInfo.allocated_page_num].isModify == 1)
            return (j + num) % pInfo.allocated_page_num;
        pageList[(j + num) % pInfo.allocated_page_num].isVisit = 0;
    }
    // 3.若第二轮扫描失败，则重新扫描，查找第一个(0,0）的帧用于替换。本轮扫描不修改任何标志位
    for (j = 0; j < pInfo.allocated_page_num; j++){
        if (pageList[(j + num) % pInfo.allocated_page_num].isVisit == 0 && pageList[(j + num) % pInfo.allocated_page_num].isModify == 0)
            return (j+num) % pInfo.allocated_page_num;
    }
    //4. 若第三轮扫描失败，则重新扫描，查找第一个(0,1）的帧用于替换。
    for (j = 0; j < pInfo.allocated_page_num; j++){
        if (pageList[(j + num) % pInfo.allocated_page_num].isVisit == 0 && pageList[(j + num) % pInfo.allocated_page_num].isModify == 1)
            return (j + num) % pInfo.allocated_page_num;
        // pageList[(j + num) % pInfo.allocated_page_num].isVisit = 0;
    }
}

int replace_page_clock(int num) {
    int j;
    // 将所有可能被置换的页面排成一个循环队列,用（访问位，修改位）的形式表示各页面状态
    // 1.当需要淘汰一个页面时，只需检查页的访问位。如果是0，就选择该页换出;如果是1，则将它置为0，暂不换出，继续检查下一个页面
    for (j = 0; j < pInfo.allocated_page_num; j++){
        if (pageList[(j + num) % pInfo.allocated_page_num].isVisit == 0 )
            return (j+num) % pInfo.allocated_page_num;
        pageList[(j + num) % pInfo.allocated_page_num].isVisit = 0;
    }
    //2. 若第一轮扫描中所有页面都是1，则将这些页面的访问位依次置为0后，再进行第二轮扫描（第二轮扫描中一定会有访问位为0的页面
    for (j = 0; j < pInfo.allocated_page_num; j++){
        if (pageList[(j + num) % pInfo.allocated_page_num].isVisit == 0 )
            return (j + num) % pInfo.allocated_page_num;
    }
}
//随机化函数
bool randBool()
{
    if ((rand() % 2 + 1) == 1)
        return true;
    else
        return false;
}


void CLOCK_pro(int choose)
{
    int n, full, status;
    int num = -1;
    replace_page = 0;          // 页面替换指针初始化为0
    page_loss_num = 0;  // 缺页数初始化为0

    full = 0;           // 是否装满是所有的页框
    for (n = 0; n < pInfo.allocated_page_num; n++) // 清除页框信息
        {
        pageList[n].pages = -1;
        pageList[n].isModify = 0;
        pageList[n].isVisit = 0;
        pageList[n].time = 0;
        }
    is_loss_page = 0;   //缺页标志，0为不缺页，1为缺页
    for (current_page = 0; current_page < pInfo.visit_list_length; current_page++)  // 执行算法
        {
        status = isExist(pInfo.pages[current_page]);  //查找页面是否在内存
        if (full < pInfo.allocated_page_num)    // 开始时不计算缺页
        {
            if (status == 0)    // 页不存在则装入页面
            {
                pageList[replace_page].pages = pInfo.pages[current_page];
                replace_page = (replace_page + 1) % pInfo.allocated_page_num;
                pageList[n].isVisit = 1;
                full++;
            }
        }
        else      // 正常缺页置换
        {
            if (status == 0)    // 页不存在则置换页面
                {
                    if(choose==1)
                        replace_page = replace_page_pro(++num); // 当choose =1时采用改进的clock算法
                    else if(choose==0)
                        replace_page = replace_page_clock(++num); // 当choose =0时采用基本的clock算法
                    pageList[replace_page].pages = pInfo.pages[current_page];
                    pageList[replace_page].isVisit = 1;
                    pInfo.page_missing_num++;     // 缺页次数加1
                }
        }
        Sleep(10);
            showState();       // 显示当前状态
        }	 // 置换算法循环结束
        _getch();
    return;
}


//  LRU页面置换算法
void LRU(void)
{
    int n, full, status, max;
    replace_page = 0;          // 页面替换指针
    page_loss_num = 0;  // 缺页次数初始化为0

    full = 0;           // 是否装满所有的页框
    for (n = 0; n < pInfo.allocated_page_num; n++)
    {
        pageList[n].pages = -1;    // 清除页框信息
        pageList[n].time = 0;   // 清除页框历史
    }
    is_loss_page = 0;    //缺页标志，0为不缺页，1为缺页
    for (current_page = 0; current_page < pInfo.visit_list_length; current_page++)  // 执行算法
        {
        status = isExist(pInfo.pages[current_page]);  //查找页面是否在内存
        if (full < pInfo.allocated_page_num)   // 开始时不计算缺页
            {
            if (status == 0)   // 页不存在则装入页面
                {
                pageList[replace_page].pages = pInfo.pages[current_page]; //把要调入的页面放入一个空的页框里
                replace_page = (replace_page + 1) % pInfo.allocated_page_num;
                full++;
                }
            }
        else //正常缺页置换
        {
            if (status == 0)//页不存在则置换页面
                {
                max = 0;
                for (n = 1; n < pInfo.allocated_page_num; n++)
                {
                    if (pageList[n].time > pageList[max].time)
                    {
                        max = n;
                    }
                }
                replace_page = max;
                pageList[replace_page].pages = pInfo.pages[current_page];
                pageList[replace_page].time = 0;
                pInfo.page_missing_num++;  // 缺页次数加1
                }
        }
        Sleep(10);
            showState();    // 显示当前状态
        } 	// 置换算法循环结束
        _getch();
    return;
}



//  主函数

int main()
{
    char ch;
    system("cls");
    printf("**********页面置换算法********\n");
    int m,e,s;
    printf("请输入工作集移动率(m):");
    scanf("%d",&m); //8
    printf("请输入工作集包含的页数(e):");
    scanf("%d",&e); //8
    printf("请输入重复次数(s):");
    scanf("%d",&s);  // 10
    getRandomList(m, e, s);  // 随机生成访问序列
    clock_t start, finish;
    double totaltime;
    while (true)
    {
        printf("====================MENU=========================\n");
        printf("1. 最佳淘汰算法(OPT)\n");
        printf("2. 先进先出淘汰算法(FIFO)\n");
        printf("3. 最近最久未使用淘汰算法(LRU)\n");
        printf("4. 基本的Clock 淘汰算法\n");
        printf("5. 改进型的Clock 淘汰算法\n");
        printf("=================================================\n");
        printf("请输入选择的算法(1/2/3/4/5): ");
        ch = (char)_getch();
        switch (ch) {
            case '1':
                printf("\n《------------1. 最佳淘汰算法(OPT)--------------》\n");
                start = clock();
                OPT();
                finish = clock();
                totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
                cout << "\n运行总时长= " << totaltime << " 秒" << endl;
                break;
            case '2':
                printf("\n\n《----------2. 先进先出淘汰算法(FIFO)------------》\n");
                start = clock();
                FIFO();
                finish = clock();
                totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
                cout << "\n运行总时长= " << totaltime << " 秒" << endl;
                break;
            case '3':
                printf("\n\n《--------3. 最近最久未使用淘汰算法(LRU)-----------》\n");
                start = clock();
                LRU();
                finish = clock();
                totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
                cout << "\n运行总时长= " << totaltime << " 秒" << endl;
                break;
            case '4':
                printf("\n\n《------------ 4. Clock 淘汰算法--------------》\n");
                start = clock();
                CLOCK_pro(0);
                finish = clock();
                totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
                cout << "\n运行总时长= " << totaltime << "秒" << endl;
                break;
            case '5':
                printf("\n\n《------------ 5. 改进的Clock 淘汰算法--------------》\n");
                start = clock();
                CLOCK_pro(1);
                finish = clock();
                totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
                cout << "\n运行总时长= " << totaltime << "秒" << endl;
                break;
            default:
                return 0;
        }
    }
    return 0;
}

