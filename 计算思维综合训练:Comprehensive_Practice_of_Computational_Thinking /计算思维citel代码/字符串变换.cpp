#include<iostream>
#include<string>
using namespace std;
int main()
{
	string str1, str2, str3, str4;
	int c = 1, n1, n2, d, j;
	while (1){
		str1 = str2 = "";
		getline(cin, str1);
		getline(cin, str2);
		if (str1 == "")
			break;
		n1 = str1.size();
		n2 = str2.size();
		j = 0;
		if (str1 == str2){
			printf("Case %d: Yes\n", c++);
			continue;
		}
		if (n1 != n2) {
			printf("Case %d: No\n", c++);
			continue;
		}
		else{
			str3 = str4 = "";
			for (int i = 0; i < n1; i++){
				d = str3.size();
				if (str1[i] == str2[j]){
					str4 += str2[j++];
					continue;
				}
				else{
					if (d != 0 && str2[j] == str3[--d]) {
						str4 += str2[j++];
						str3[d] = '\n';
						if (i = n1 - 1)
							str3 += str1[i];
						continue;
					}
				}
				str3 += str1[i];
			}
			d = str3.size();
			if (d != 0)
				for (d = d - 1; d >= 0; --d) {
					if (str4.size() == n1)
						break;
					str4 += str3[d];
				}
			if (str4 == str2)
				printf("Case %d: Yes\n", c++);
			else
				printf("Case %d: No\n", c++);
		}
	}
	return 0;
}



