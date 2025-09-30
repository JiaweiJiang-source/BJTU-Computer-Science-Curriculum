#include<stdio.h>
#define Yes 1
#define No 0
typedef short Status;
Status FindDuplication(short* temp, short N);
int main()
{
    short N;
    long x, y;
    short temp[10];
    bool flag;
    long K = 1;
    while (scanf("%hd", &N) != EOF)
    {
        flag = false;
        printf("Case %ld:\n", K++);
        /* 以分母为基准 */
        for (y = 99999 / N; y > 0; y--)
        {
            x = y * N;
            /* 此时 x % y == 0 && x / y == N 成立 */
            temp[0] = x / 10000;
            temp[1] = x / 1000 % 10;
            temp[2] = x / 100 % 10;
            temp[3] = x / 10 % 10;
            temp[4] = x % 10;
            if (FindDuplication(temp, 5) == Yes)
            {
                continue;
            }
            temp[5] = y / 10000;
            temp[6] = y / 1000 % 10;
            temp[7] = y / 100 % 10;
            temp[8] = y / 10 % 10;
            temp[9] = y % 10;
            if (FindDuplication(temp + 5, 5) == Yes)
            {
                continue;
            }
            if (FindDuplication(temp, 10) == No)
            {
                printf("    %hd%hd%hd%hd%hd/%hd%hd%hd%hd%hd=%hd\n", temp[0], temp[1], temp[2], temp[3],
                    temp[4], temp[5], temp[6], temp[7], temp[8], temp[9], N);
                flag = true;
            }
        }
        if (flag == false)
        {
            printf("    No such numbers\n");
        }
    }
    return 0;
}
Status FindDuplication(short* temp, short N)
{
    short i, j;
    for (i = 0; i < N - 1; i++)
    {
        for (j = i + 1; j < N; j++)
        {
            if (temp[i] == temp[j])
            {
                return Yes;
            }
        }
    }
    return No;
}



