#include <bits/stdc++.h>
using namespace std;
string s1, s2;
int a[30], b[30];
int main() {
    cin >> s1 >> s2;
    for (char c : s1) {
        a[c - 'A']++;
    }
    for (char c : s2) {
        b[c - 'A']++;
    }
    sort(a, a + 26);
    sort(b, b + 26);
    int flag = 1;
    for (int i = 0; i < 26; i++) {
        if (a[i] != b[i])
            flag = 0;
    }
    cout << (flag ? "YES\n" : "NO\n");
    return 0;
}