#include<stdio.h>
int main()
{
    int n;
    int sum = 0;
    while (scanf("%x", &n) != EOF)
    {
        sum += n;
        if (getchar() == '\n')
        {
            printf("%d\n", sum);
            sum = 0;
        }
    }
    return 0;
}