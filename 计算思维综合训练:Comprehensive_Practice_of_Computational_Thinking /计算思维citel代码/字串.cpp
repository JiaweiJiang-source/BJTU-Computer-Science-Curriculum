#include<stdio.h>
#include<string.h>
#define MAXSIZE 100
int main()
{
    char data[MAXSIZE + 1];
    char* temp = data;
    /* ��ȫ�ؽ�������䵽�ڴ滺������ */
    fgets(data, sizeof(data), stdin);
    while (*temp != '\n' && *temp != '\0')
    {
        temp++;
    }
    *temp = '\0';
    /* */
    short N;
    short offset;
    short i, j, k;
    short len;
    short K = 1;
    while (scanf("%hd", &N) != EOF)
    {
        printf("Case %hd:\n", K++);
        /* ��NΪ����ָ��ַ��� */
        len = strlen(data);
        for (offset = 1; offset <= N; offset++)
        {
            /* �����offset���ַ� */
            for (k = 0; k < offset; k++)
            {
                putchar(data[k]);
            }
            putchar(' ');
            for (j = offset; j < len; j += N)
            {
                for (i = j; i < len && i < j + N; i++)
                {
                    putchar(data[i]);
                }
                if (j + N < len)
                {
                    putchar(' ');
                }
            }
            putchar('\n');
        }
    }
    return 0;
}



