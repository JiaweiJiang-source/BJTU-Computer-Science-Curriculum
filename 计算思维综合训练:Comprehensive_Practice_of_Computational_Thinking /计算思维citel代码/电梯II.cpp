#include<bits/stdc++.h>
using namespace std;
int main()
{	
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int num = 1;
    int step1,step2;
	int n, u, d, s;
	while (scanf("%d %d %d %d", &n, &u, &d, &s) == 4){
		priority_queue<int, vector<int>, less<int>> que1,que3,que5,que7;
		priority_queue<int, vector<int>, greater<int>> que2,que4,que6,que8;
		int from, to;
	    int upfrom_min1 = 205,upfrom_min2=205, downfrom_max1 = -1,downform_max2=-1;
		char ch;
		do {
			scanf("%d-%d", &from, &to);
			if (to > from){
				if(from >= n)
                    que1.push(to);
				if(from >= n)
                    que1.push(from);
				if (from < n)
                    que3.push(to);
				if (from < n)
                    que3.push(from);
				if (from <= n)
                    upfrom_min1 = min(upfrom_min1, from);
				if (from < n)
                    upfrom_min2 = min(upfrom_min2, from);
			}
			else{
				if (from <= n)
                    que2.push(to);
				if (from <= n)
                    que2.push(from);
				if (from > n)
                    que4.push(to);
				if (from > n)
                    que4.push(from);
				if (from >= n)
                    downfrom_max1 = max(downfrom_max1, from);
				if (from > n)
                    downform_max2 = max(downform_max2, from);
			}
		} while ((ch = getchar()) != '\n' && ch != EOF);
		int time1=0, time2=0;
		step1 = step2 = n;
		if (!que1.empty()){
			time1 += u * (que1.top() - n);
			step1 = que1.top();
		}
		while (!que1.empty()){
			int x = que1.top();
			que5.push(x);
			if(x!=n)time1 += s;
			while (!que1.empty() && que1.top() == x)que1.pop();
		}
		if (downfrom_max1 != -1 && downfrom_max1 > step1) {
			time1 += u * (downfrom_max1 - step1) + s;
			step1 = downfrom_max1;
		}
		int step3 = step1;
		if (!que2.empty()){
			time1 += d * (step3 - que2.top());
			time2 += d * (n - que2.top());
			step2 = que2.top();
			step3 = que2.top();
		}
		while (!que2.empty()){
			int x = que2.top();
			que6.push(x);
			if (x != n)time2 += s;
			while (!que2.empty() && que2.top() == x)
                que2.pop();
		}
		if (upfrom_min1 != 205 && upfrom_min1 < step2) {
			time2 += d * (step2 - upfrom_min1) + s;
			step2 = upfrom_min1;
		}
		que2 = que6;
		int step4 = step2;
		if (!que4.empty()&&step3>que4.top()){
			time1 += d * (step3 - que4.top());
			step3 = que4.top();
		}
		que8 = que4;
		while (!que4.empty()){
			que2.push(que4.top());
			que4.pop();
		}
		while (!que2.empty()){
			int x = que2.top();
			if (x != step1)
                time1 += s;
			while (!que2.empty() && que2.top() == x)
                que2.pop();
		}
		if (upfrom_min2 != 205 && step3 > upfrom_min2) {
			time1 += d * (step3 - upfrom_min2) + s;
			step3 = upfrom_min2;
		}
		if (!que3.empty())
            time1 += u * (que3.top() - step3);
		while (!que3.empty()){
			int x = que3.top();
			que7.push(x);
			if (x != step3)time1 += s;
			while (!que3.empty() && que3.top() == x)
                que3.pop();
		}
		while (!que5.empty()){
			que7.push(que5.top());
			que5.pop();
		}
		if (!que7.empty())
            step4 = que7.top();
		time2 += u * (step4 - step2);
		while (!que7.empty()){
			int x = que7.top();
			if (x != step2)
                time2 += s;
			while (!que7.empty() && que7.top() == x)
                que7.pop();
		}
		if (downform_max2 != -1 && downform_max2 > step4) {
			time2 += u * (downform_max2 - step4) + s;
			step4 = downform_max2;
		}
		if (!que8.empty())
            time2 += d * (step4-que8.top());
		while (!que8.empty()){
			int x = que8.top();
			if (x != step4)
                time2 += s;
			while (!que8.empty() && que8.top() == x)
                que8.pop();
		}
		int ans = min(time1, time2);
		cout << "Case " << num++ << ": " << ans << endl;
	}
}
