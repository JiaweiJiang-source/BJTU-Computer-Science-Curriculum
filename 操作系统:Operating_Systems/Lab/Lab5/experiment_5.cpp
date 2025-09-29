#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
const int maxRightValue = 199;//磁头移动的最右边界值
const int minLeftValue = 0;//磁头移动的最左边界值
vector<int> FSCAN_SHOW;//存放FSCAN_SHOW算法的访问顺序

void getRandomNumber(vector<int>& randValue, int& pos)
{
	
	for (int i = 0; i < 100; i++)
		randValue.push_back(rand() % 200);
	pos = rand() % 200;
}
//输入 请求访问序列和磁头所在位置
//输出 移动磁道数
void Show(const string& str, const vector<int>& t)
{
	cout << str << "的访问序列为:";
	for (int i = 0; i < t.size(); i++)
	{
		if (i != t.size() - 1)
			cout << t[i] << "->";
		else
			cout << t[i] << endl;
	}
}

int FCFS(const vector<int>& t, int pos)
{
	int sum = 0;
	Show("FCFS", t);
	for (int i = 0; i < t.size(); i++)
	{
		sum += abs(pos - t[i]);
		pos = t[i];
	}
	return sum;
}
int findClose(const vector<int>& t, int pos)
{
	int minDistance = INT_MAX;
	int index = -1;
	for (int i = 0; i < t.size(); i++)
	{
		if (t[i] == -1)continue;
		int distance = abs(pos - t[i]);
		if (minDistance > distance)
		{
			minDistance = distance;
			index = i;
		}
	}
	return index;
}
int SSTF(vector<int> t, int pos)
{
	vector<int> show;
	int sum = 0;
	for (int i = 0; i < t.size(); i++)
	{
		int index = findClose(t, pos);
		if (index == -1)
		{
			break;
		}
		else
		{
			show.push_back(t[index]);
			sum += abs(pos - t[index]);
			pos = t[index];
			t[index] = -1;
		}
	}
	Show("SSTF", show);
	return sum;
}
int FindLarger(vector<int>& t, int pos)
{
	int index = -1;
	for (int i = 0; i < t.size(); i++)
	{
		if (pos <= t[i])
		{
			index = i;
			break;
		}
	}
	return index;
}
//假设磁盘指针总是向右移动
pair<int, int> SCAN(vector<int> t, int pos, int flag = 1)
{
	int sum = 0;
	vector<int> left, right;
	vector<int> show;
	for (auto e : t)
	{
		if (e < pos)
		{
			left.push_back(e);
		}
		else
		{
			right.push_back(e);
		}
	}
	sort(left.begin(), left.end());
	sort(right.begin(), right.end());
	for (auto e : right)
	{
		show.push_back(e);
	}
	sum += maxRightValue - pos;
	if (!left.empty())
	{
		sum += maxRightValue - left[0];
		reverse(left.begin(), left.end());

		for (auto e : left)
		{
			show.push_back(e);
		}
	}
	if (flag)
	{
		Show("SCAN", show);
	}
	else
	{
		for (auto e : show)
		{
			FSCAN_SHOW.push_back(e);
		}
	}
	pair<int, int> res;
	res.first = sum;
	res.second = pos;
	return res;
}
int CSCAN(vector<int> t, int pos)
{
	int sum = 0;
	vector<int> left, right;
	vector<int> show;
	for (auto e : t)
	{
		if (e < pos)
		{
			left.push_back(e);
		}
		else
		{
			right.push_back(e);
		}
	}
	sort(left.begin(), left.end());
	sort(right.begin(), right.end());
	for (auto e : right)
	{
		show.push_back(e);
	}
	sum += maxRightValue - pos + 200;
	if (!left.empty())
	{
		for (auto e : left)
		{
			show.push_back(e);
		}
		sum += left.back();
	}
	Show("CSCAN", show);

	return sum;
}
//将前50个请求划分为当前正在进行的队列，后50个请求划分为扫描期间请求磁盘调度的进程

int FSCAN(vector<int> t, int pos)
{
	int sum = 0;
	vector<int> t1(t.begin(), t.begin() + 50);
	vector<int> t2(t.begin() + 50, t.begin() + 100);
	const pair<int, int>& temp = SCAN(t1, pos, 0);
	sum += temp.first;
	sum += SCAN(t2, temp.second, 0).first;
	Show("FSCAN", FSCAN_SHOW);
	return sum;
}
int main()
{

	int pos = 0;
	vector<int> randValue;
	srand((unsigned)time(NULL));
	getRandomNumber(randValue, pos);

	cout << "FCFS的平均寻道数为:" << FCFS(randValue, pos) / 100 << endl;
	cout << endl;

	cout << "SSTF的平均寻道数为:" << SSTF(randValue, pos) / 100 << endl;
	cout << endl;

	cout << "SCAN的平均寻道数为:" << SCAN(randValue, pos).first / 100 << endl;
	cout << endl;

	cout << "CSCAN的平均寻道数为:" << CSCAN(randValue, pos) / 100 << endl;
	cout << endl;

	cout << "FSCAN的平均寻道数为:" << FSCAN(randValue, pos) / 100 << endl;
	cout << endl;
	return 0;
}
