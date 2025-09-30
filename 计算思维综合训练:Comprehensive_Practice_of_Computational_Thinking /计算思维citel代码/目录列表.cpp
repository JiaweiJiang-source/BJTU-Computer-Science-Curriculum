#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n, wid;
    while (cin >> n >> wid)
    {
        vector<string> name(n);
        int maxlen = 0;
        for (int i = 0; i < n; i++)
        {
            cin >> name[i];
            maxlen = max(maxlen, (int)name[i].length());
        }

        int num1 = (wid + 2) / (maxlen + 2);
        int num2 = n / num1 + 1;
        int last = num1 + n - num2 * num1;
        sort(name.begin(), name.end());
        for (int i = 1; i <= wid; i++)
        {
            cout << '-';
        }
        cout << "\n";
        for (int i = 1; i <= num2; i++)
        {
            for (int j = 1; j <= num1; j++)
            {
                if (((i - 1) * num1 + j - 1) < n)
                {
                    int here;
                    if (j <= last)
                    {
                        here = (j - 1) * num2 + i - 1;
                    }
                    else
                    {
                        here = last * num2 + (j - last - 1) * (num2 - 1) + i - 1;
                    }
                    cout << name[here];
                    if (j == num1 || (j == last && i == num2))
                    {
                        cout << "\n";
                    }
                    else
                    {
                        for (int i = 1; i <= maxlen - name[here].length(); i++)
                        {
                            cout << " ";
                        }
                        cout << "  ";
                    }
                }
            }
        }
    }
    return 0;
}