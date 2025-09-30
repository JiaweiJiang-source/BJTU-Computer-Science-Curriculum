#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>

// 定义学生结构
typedef struct {
    char szName[20];   // 姓名
    double dScore[2];  // 两门课的成绩
    double avgScore;   // 两门课的平均成绩
} STUD;

// 计算并返回班级中最低平均分，同时打印具有最低平均分的学生的信息
double studLowestAvgScore(STUD* pStu, int nLen) {
    double lowestAvg = pStu[0].dScore[0] + pStu[0].dScore[1]; // 初始化最低平均分为第一个学生的总分
    int lowestIndex = 0; // 记录最低平均分的学生的索引

    // 计算每个学生的平均成绩，并找出最低平均分
    for (int i = 0; i < nLen; i++) {
        pStu[i].avgScore = (pStu[i].dScore[0] + pStu[i].dScore[1]) / 2.0;

        if (pStu[i].avgScore < lowestAvg) {
            lowestAvg = pStu[i].avgScore;
            lowestIndex = i;
        }
    }

    // 打印具有最低平均分的学生的信息
    printf("具有最低平均分的学生的信息:\n");
    printf("姓名: %s\n", pStu[lowestIndex].szName);
    printf("第一门课成绩: %.2lf\n", pStu[lowestIndex].dScore[0]);
    printf("第二门课成绩: %.2lf\n", pStu[lowestIndex].dScore[1]);
    printf("平均成绩: %.2lf\n", pStu[lowestIndex].avgScore);

    return lowestAvg;
}

int main() {
    int nLen; // 学生人数
    STUD students[10]; // 定义学生数组，最多10个学生
    char line[100]; // 用于存储每行输入

    // 输入学生人数
    printf("请输入学生人数 (0 < nLen <= 10): ");
    scanf_s("%d", &nLen);
    getchar(); // 读取换行符，避免影响后续输入

    // 输入每个学生的信息
    printf("请输入每个学生的姓名和两门课的成绩:\n");
    for (int i = 0; i < nLen; i++) {
        printf("第 %d 个学生: ", i + 1);
        fgets(line, sizeof(line), stdin); // 读取一行输入
        sscanf(line, "%s %lf %lf", students[i].szName, &students[i].dScore[0], &students[i].dScore[1]);
    }

    // 调用函数计算并输出最低平均分
    double lowestAvg = studLowestAvgScore(students, nLen);
    printf("所有学生中的最低平均分为: %.2lf\n", lowestAvg);

    return 0;
}