while True:
	a, b = map(int, input().split())
	print(a + b)//用python计算超大数很方便
/*#include <bits/stdc++.h>
using namespace std;
string add(string a, string b) {
    int la = a.length(), lb = b.length(), lm = max(la, lb);
    for (int i = lb; i < lm; ++i)
        b = "0" + b;
    for (int i = la; i < lm; ++i)
        a = "0" + a;
    string c;
    int jw = 0;
    int s;
    for (int i = lm - 1; i >= 0; --i) {
        s = a[i] - '0' + b[i] - '0' + jw;
        jw = s / 10;
        c = char(s % 10 + '0') + c;
    }
    if (jw)
        c = char(jw + '0') + c;
    return c;
}
bool absBigger(string a, string b) {
    if (a[0] == '-')
        a = a.substr(1);
    if (b[0] == '-')
        b = b.substr(1);
    if (a.length() != b.length())
        return a.length() > b.length();
    for (int i = 0; i < a.length(); i++) {
        if (a[i] != b[i])
            return a[i] > b[i];
    }
    return false;
}
string sub(string a, string b) {
    bool sw = false;
    if (absBigger(b, a))
        swap(a, b), sw = true;
    for (int i = b.length(); i < a.length(); i++) {
        b = "0" + b;
    }
    string c = "";
    for (int i = 0; i < a.length(); i++)
        c = "0" + c;
    for (int i = a.length() - 1; i >= 0; i--) {
        if (a[i] >= b[i])
            c[i] = a[i] - '0' - (b[i] - '0') + '0';
        else {
            a[i - 1]--;
            c[i] = a[i] - '0' + 10 - (b[i] - '0') + '0';
        }
    }
    while (c[0] == '0')
        c = c.substr(1);
        if(c=="")c="0";
    if (sw)
        return "-" + c;
    return c;
}
int main() {
    ios::sync_with_stdio(false);
    string a, b;
    while (cin >> a >> b) {
        if (a[0] != '-' && b[0] != '-')
            cout << add(a, b) << endl;
        else if (a[0] == '-' && b[0] == '-') {
            cout << "-" << add(a.substr(1), b.substr(1)) << endl;
        }
        else if (a[0] == '-' && b[0] != '-') {
            cout << sub(b, a.substr(1)) << endl;
        }
        else
            cout << sub(a, b.substr(1)) << endl;
        // cout << sub(a, b) << endl;
    }
    return 0;
}*/
//这是一位大佬的c++写法