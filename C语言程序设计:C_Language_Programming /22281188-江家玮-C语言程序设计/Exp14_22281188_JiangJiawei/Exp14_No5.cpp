#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>

// 定义书的结构
typedef struct {
    char Name[50]; // 书名
    double Price;  // 价钱
    int Pages;     // 页数
} BOOK;

// 找出页数最多的一本书并打印其信息，返回这本书的页数
int MaxPages(BOOK* pBook, int nLen) {
    int maxPages = pBook[0].Pages; // 假设第一本书的页数最多
    int maxIndex = 0;             // 记录页数最多的书的索引

    // 遍历所有书，找出页数最多的一本
    for (int i = 1; i < nLen; i++) {
        if (pBook[i].Pages > maxPages) {
            maxPages = pBook[i].Pages;
            maxIndex = i;
        }
    }

    // 打印页数最多的一本书的信息
    printf("页数最多的一本书是: %s\n", pBook[maxIndex].Name);
    printf("价钱: %.2lf\n", pBook[maxIndex].Price);
    printf("页数: %d\n", pBook[maxIndex].Pages);

    return maxPages;
}

// 求出这批书的平均页数，返回该平均页数
double MeanPages(BOOK* pBook, int nLen) {
    int totalPages = 0;

    // 计算所有书的页数总和
    for (int i = 0; i < nLen; i++) {
        totalPages += pBook[i].Pages;
    }

    // 计算平均页数
    return (double)totalPages / nLen;
}

int main() {
    int nLen; // 书的数量
    BOOK books[10]; // 定义书的数组，最多10本书
    char line[100]; // 用于存储每行输入

    // 输入书的数量
    printf("请输入书的数量 (0 < nLen <= 10): ");
    scanf("%d", &nLen);
    getchar(); // 读取换行符，避免影响后续输入

    // 输入每本书的信息
    printf("请输入每本书的书名、价钱和页数:\n");
    for (int i = 0; i < nLen; i++) {
        printf("第 %d 本书: ", i + 1);
        fgets(line, sizeof(line), stdin); // 读取一行输入
        sscanf(line, "%s %lf %d", books[i].Name, &books[i].Price, &books[i].Pages);
    }

    // 找出页数最多的一本书并打印其信息
    int maxPages = MaxPages(books, nLen);
    printf("页数最多的一本书的页数是: %d\n", maxPages);

    // 计算并输出平均页数
    double meanPages = MeanPages(books, nLen);
    printf("这批书的平均页数是: %.2lf\n", meanPages);

    return 0;
}
//#include <stdio.h>
//#include <string.h>
//
//// 定义书的结构
//typedef struct {
//    char Name[50]; // 书名
//    double Price;  // 价钱
//    int Pages;     // 页数
//} BOOK;
//
//// 找出页数最多的一本书并打印其信息，返回这本书的页数
//int MaxPages(BOOK* pBook, int nLen) {
//    int maxPages = pBook[0].Pages; // 假设第一本书的页数最多
//    int maxIndex = 0;             // 记录页数最多的书的索引
//
//    // 遍历所有书，找出页数最多的一本
//    for (int i = 1; i < nLen; i++) {
//        if (pBook[i].Pages > maxPages) {
//            maxPages = pBook[i].Pages;
//            maxIndex = i;
//        }
//    }
//
//    // 打印页数最多的一本书的信息
//    printf("页数最多的一本书是: %s\n", pBook[maxIndex].Name);
//    printf("价钱: %.2lf\n", pBook[maxIndex].Price);
//    printf("页数: %d\n", pBook[maxIndex].Pages);
//
//    return maxPages;
//}
//
//// 求出这批书的平均页数，返回该平均页数
//double MeanPages(BOOK* pBook, int nLen) {
//    int totalPages = 0;
//
//    // 计算所有书的页数总和
//    for (int i = 0; i < nLen; i++) {
//        totalPages += pBook[i].Pages;
//    }
//
//    // 计算平均页数
//    return (double)totalPages / nLen;
//}
//
//int main() {
//    int nLen; // 书的数量
//    BOOK books[10]; // 定义书的数组，最多10本书
//    char line[100];
//
//    // 输入书的数量
//    printf("请输入书的数量 (0 < nLen <= 10): ");
//    scanf_s("%d", &nLen);
//    getchar(); // 读取换行符
//
//    // 输入每本书的信息
//    printf("请输入每本书的书名、价钱和页数:\n");
//    for (int i = 0; i < nLen; i++) {
//        printf("第 %d 本书: ", i + 1);
//        fgets(line, sizeof(line), stdin); // 读取一行输入
//        sscanf_s(line, "%s %lf %d", books[i].Name, &books[i].Price, &books[i].Pages);
//        //getchar(); // 读取换行符
//    }
//
//
//    // 找出页数最多的一本书并打印其信息
//    int maxPages = MaxPages(books, nLen);
//    printf("页数最多的一本书的页数是: %d\n", maxPages);
//
//    // 计算并输出平均页数
//    double meanPages = MeanPages(books, nLen);
//    printf("这批书的平均页数是: %.2lf\n", meanPages);
//
//    return 0;
//}