#include <stdio.h>

int main() {
    // 定义基本类型的变量
    char c;
    int i;
    float f;
    double d;

    // 定义指针类型的变量
    char* pc;
    int* pi;
    float* pf;
    double* pd;

    // 为基本类型的变量赋值或输入值
    printf("请输入一个字符: ");
    scanf_s(" %c", &c); // 注意前面的空格，用于跳过换行符

    printf("请输入一个整数: ");
    scanf_s("%d", &i);

    printf("请输入一个浮点数: ");
    scanf_s("%f", &f);

    printf("请输入一个双精度浮点数: ");
    scanf_s("%lf", &d);

    // 为指针类型的变量赋值
    pc = &c; // 指向字符变量的地址
    pi = &i; // 指向整数变量的地址
    pf = &f; // 指向浮点数变量的地址
    pd = &d; // 指向双精度浮点数变量的地址

    // 输出表头
    printf("\nVarName\t\tType\t\tSize\t\tAddress\t\t\tValue\n");
    printf("-------\t\t----\t\t----\t\t--------\t\t-----\n");

    // 输出基本类型变量的属性
    printf("c\t\tchar\t\t%lu\t\t%pH\t\t%c\n", sizeof(c), &c, c);
    printf("i\t\tint\t\t%lu\t\t%pH\t\t%d\n", sizeof(i), &i, i);
    printf("f\t\tfloat\t\t%lu\t\t%pH\t\t%10.2f\n", sizeof(f), &f, f);
    printf("d\t\tdouble\t\t%lu\t\t%pH\t\t%10.2lf\n", sizeof(d), &d, d);

    // 输出指针类型变量的属性
    printf("pc\t\tchar*\t\t%lu\t\t%pH\t\t%pH\n", sizeof(pc), &pc, pc);
    printf("pi\t\tint*\t\t%lu\t\t%pH\t\t%pH\n", sizeof(pi), &pi, pi);
    printf("pf\t\tfloat*\t\t%lu\t\t%pH\t\t%pH\n", sizeof(pf), &pf, pf);
    printf("pd\t\tdouble*\t\t%lu\t\t%pH\t\t%pH\n", sizeof(pd), &pd, pd);

    return 0;
}