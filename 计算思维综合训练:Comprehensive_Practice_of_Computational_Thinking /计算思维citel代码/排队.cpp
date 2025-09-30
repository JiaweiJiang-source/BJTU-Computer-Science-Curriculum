#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
void solve(int n)
{
    map<string, int> zuhao;
    list<int> group;
    vector<queue<string>> run(n + 1);
    for(int i = 1; i <= n; i++)
    {
        int num;
        cin >> num;
        for (int j = 1; j <= num; j++)
        {
            string member;
            cin >> member;
            zuhao.insert(make_pair(member, i));
        }
    }
    string q;
    while (cin >> q && q != "stop")
    {
        if (q == "enqueue")
        {
            string tmp;
            cin >> tmp;
            int index;
            if (zuhao.count(tmp))
            {
                index = zuhao[tmp];
            }
            else
            {
                index = run.size();
                zuhao.insert(make_pair(tmp, index));
                run.resize(index + 1);
            }
            if (run[index].empty())
            {
                group.push_back(index);
            }
            run[index].push(tmp);
        }
        else if (q == "deqteam")
        {
            string tmp;
            cin >> tmp;
            int index = zuhao[tmp];
            while (!run[index].empty())
            {
                cout << run[index].front();
                run[index].pop();
                if (!run[index].empty())
                    cout << " ";
                else
                    cout << endl;
            }
            for (auto it = group.begin(); it != group.end(); it++)
            {
                if (*it == index)
                {
                    it = group.erase(it);
                    break;
                }
            }
        }
        else if (q == "dequeue")
        {
            if (group.empty())
                continue;
            int first = group.front();
            cout << run[first].front() << endl;
            run[first].pop();
            if (run[first].empty())
            {
                group.pop_front();
            }
        }
    }
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);//加快cin与cout的输入和输出
    int n, cnt = 0;
    while (cin >> n && n)
    {
        cnt++;
        cout << "Case #" << cnt << ":\n";
        solve(n);
    }
    return 0;
}