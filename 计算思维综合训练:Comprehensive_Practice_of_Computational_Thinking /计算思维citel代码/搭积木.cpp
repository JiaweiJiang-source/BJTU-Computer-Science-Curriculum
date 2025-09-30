#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
int N;
vector<int> pile[50];
int row_a, row_b, column_a, column_b;
//���ô��ݣ������õ���ʽ���ص�����
void find(int a, int& row, int& column) // row:��(���ڵĶ�) ; column:��(��ľ�ĸ߶�)
{
    for (row = 0; row < N; row++)
    {
        for (column = 0; column < pile[row].size(); column++)
        {
            if (pile[row][column] == a)
                return;
        }
    }
}
void movement(int row, int column, int row_goal)
{
    for (int i = column; i < pile[row].size(); i++)
    {
        if (row_goal == -1)  //������row_goalΪ-1����Ż�ԭ��
            pile[pile[row][i]].push_back(pile[row][i]);
        else
            pile[row_goal].push_back(pile[row][i]);
    }
    pile[row].resize(column); // row��column�߶����ϵĻ�ľ�Ѿ���Ų�ߣ�����pile�Ĵ�СΪ0~column����������Ļ�ľ
}
int main()
{
    cin >> N;
    string s1, s2;
    int a, b;
    for (int i = 0; i < N; i++)
        pile[i].push_back(i);
    for (;;)
    {
        cin >> s1;
        if (s1[0] == 'q')
            break;
        cin >> a >> s2 >> b;
        find(a, row_a, column_a);
        find(b, row_b, column_b);
        if (row_a == row_b)
            continue;
        if (s1[0] != 'x')
        {
            if (s1[0] == 'm' && column_a + 1 < pile[row_a].size())
                movement(row_a, column_a + 1, -1);
            if (s2[1] == 'n' && column_b + 1 < pile[row_b].size())
                movement(row_b, column_b + 1, -1);
            movement(row_a, column_a, row_b);
        }
        if (s1[0] == 'x')
            pile[row_a].swap(pile[row_b]);
    }
    for (int i = 0; i < N; i++)
    {
        cout << i << ":";
        for (int j = 0; j < pile[i].size(); j++)
            cout << " " << pile[i][j];
        cout << endl;
    }
    return 0;
}
