#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
using namespace std;
unordered_map<string, int> frequency;
typedef pair<string, int> PAIR;
bool cmp(const PAIR& left, const PAIR& right)
{
    if (left.second == right.second)
        return left.first < right.first;
    else
        return left.second > right.second;
}
int main()
{
    string s1;
    while (cin >> s1)
    {
        string s2 = "";
        transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
        for (int i = 0; i < s1.size(); i++)
        {
            if (s1[i] >= 'a' && s1[i] <= 'z')
                s2.push_back(s1[i]);
            else if (s1[i] == '\'')
                break;
            else
                continue;
        }
        if (s2 == "")
            continue;
        if (frequency.find(s2) == frequency.end())
            frequency.insert(make_pair(s2, 1));
        else
            frequency.find(s2)->second++;
    }
    vector<pair<string, int> > pai_xu(frequency.begin(), frequency.end());
    sort(pai_xu.begin(), pai_xu.end(), cmp);
    for (int i = 0; i < pai_xu.size(); i++)
    {
        cout << pai_xu[i].first << endl;
    }
    return 0;
}
