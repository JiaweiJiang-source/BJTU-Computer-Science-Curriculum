#include<stdio.h>
int main()
{
    short data[18] = { 0 };
    short i = 0, j, k, p;
    long Max, S;
    short minSpan;
    short L = 1;
    bool flag;
    while (scanf("%hd", data + i) != EOF)
    {
        i++;
        if (getchar() == '\n')
        {
            /* 遇到回车符, 意味着一轮输入结束 */
            /* 至此共输入了i个数据 */
            Max = data[0];
            /* printf("i = %hd\n", i); */
            for (j = 0; j < i; j++)
            {
                for (k = j; k < i; k++)
                {
                    S = 1;
                    for (p = j; p <= k; p++)
                    {
                        S *= data[p];
                    }
                    if (S > Max)
                    {
                        Max = S;
                    }
                }
            }
            /* printf("Max = %ld\n", Max); */
            /* 至此已找到最大连续乘积 */
            /* 开始匹配 */
            minSpan = 17;
            for (j = 0; j < i; j++)
            {
                for (k = j; k < i; k++)
                {
                    S = 1;
                    for (p = j; p <= k; p++)
                    {
                        S *= data[p];
                    }
                    if (S == Max)
                    {
                        /* printf("%hd-%hd\n", j, k); */
                        /* 匹配成功 */
                        if (k - j < minSpan)
                        {
                            minSpan = k - j;
                        }
                    }
                }
            }
            /* printf("minSpan = %hd\n", minSpan); */
            /* 至此确定了最小跨度 */
            flag = false;
            for (j = 0; j < i && flag == false; j++)
            {
                for (k = j; k < i && flag == false; k++)
                {
                    S = 1;
                    for (p = j; p <= k; p++)
                    {
                        S *= data[p];
                    }
                    if (S == Max)
                    {
                        if (k - j == minSpan)
                        {
                            flag = true;
                            printf("Case %hd: ", L++);
                            printf("%ld ", Max);
                            printf("%hd-%hd\n", j, k);
                        }
                    }
                }
            }
            /* */
            i = 0;
        }
    }
    return 0;
}



