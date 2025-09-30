#include<iostream>
#include<algorithm>
#include<map>
using namespace std;
string str1[1000], str4[1000];
string str2, str3;
map<string, string>ma;
int ans = 0;
int bns = 0;
int main() {
	cin >> str2;
	while (str2 != "#") {
		str3 = str2;
		transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
		str1[ans] += str2;
		++ans;
		sort(str2.begin(), str2.end());
		if (ma[str2] == "")ma[str2] = str3;
		else {
			if (ma[str2] != str3)
				str4[bns++] = ma[str2];
		}
		cin >> str2;
	}
	sort(str4, str4 + bns);
	int y = unique(str4, str4 + bns) - str4;
	for (int i = 0; i < y; i++)
		cout << str4[i] << endl;
	return 0;
}