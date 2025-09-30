#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>

// 定义学生结构
typedef struct {
    char szName[20];   // 姓名
    double dScore[2];  // 两门课的成绩
} STUDENT;

// 判断学生是否至少有一门课不及格
int isFail(const STUDENT* student) {
    return (student->dScore[0] < 60 || student->dScore[1] < 60);
}

int main() {
    int nLen;           // 学生人数
    STUDENT students[10]; // 学生数组，最多10个学生
    int failCount = 0;  // 不及格人数
    char line[100];     // 用于存储每行输入

    // 输入学生人数
    printf("请输入学生人数: ");
    scanf("%d", &nLen);
    getchar(); // 读取换行符，避免影响后续输入
    //printf("%d\n", nLen);
    // 输入每个学生的信息
    for (int i = 0; i < nLen; i++) {
        printf("请输入第 %d 个学生的姓名和两门课的成绩: ", i + 1);
        fgets(line, sizeof(line), stdin); // 读取一行输入
        sscanf(line, "%s %lf %lf", students[i].szName, &students[i].dScore[0], &students[i].dScore[1]);
        //getchar();
    }

    // 统计至少有一门课不及格的学生人数
    for (int i = 0; i < nLen; i++) {
        if (isFail(&students[i])) {
            failCount++;
        }
    }

    // 输出结果
    printf("至少有一门课不及格的学生人数为: %d\n", failCount);

    return 0;
}