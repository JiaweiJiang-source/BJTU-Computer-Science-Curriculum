#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include<string>
#include<iterator>
#include <sstream>
#pragma warning(disable:4996)
using namespace std;
set<string> prod; // 产生式集合
map<string, set<string> > splProd; // 分解的产生式集合
string start; // 开始符号
set<string> Vt; // 终结符集合
set<string> Vn; // 非终结符集合
set<string> VtPlus; // 终结符集合 + #
map<string, set<string> > first; // first集
map<string, set<string> > follow; // follow集

string ptr; // 当前词法单元对应的字符
string RecValue; // 当前词法单元对应的实际值

struct Item {
    int finish = 0;
    int pos = 3; // 圆点位置
    string prod;
};
struct status {
    int number;
    list<Item> Itemlist;
};  // 状态
list<status> DFA;
Item firstItem;  // 初始项目I0

struct ActionNode {
    int I;  // 当前状态
    string Vt;  // 终结符
    string act;  // r||s
    int nextI_P;  // 下一状态/规约产生式编号
};
struct GotoNode {
    int I;  // 当前状态
    string Vn;  // 非终结符
    int next_I;  // 转移状态
};
list<ActionNode> ACTION;
list<GotoNode> GOTO;

struct varX {
    string X; // 符号
    string value; // 符号的值/Temp
};  // 符号
struct anaNode {
    int S;  // 状态
    varX X;  // 符号
};
vector<anaNode> stack; // 分析栈(符号栈和状态栈)
vector<string> leftString; // 剩余输入串
vector<string> temp; // 临时记录输入串
vector<string> recValue; // 临时记录实际输入串
vector<string> recleftValue; // 剩余实际输入串

struct quaterExp {
    string op;
    string arg1;
    string arg2;
    string result;
};
list<quaterExp> Exp; // 四元式
list<int> quaterI; // 可产生四元式的状态
int recTemp = 0; // 记录result编号