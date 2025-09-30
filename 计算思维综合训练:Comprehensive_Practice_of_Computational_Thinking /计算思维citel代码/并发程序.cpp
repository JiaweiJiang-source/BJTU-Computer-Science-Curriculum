#include<bits/stdc++.h>
#define FOR(i, n) for(int i = 0; i < n; ++i)
typedef long long ll;
using namespace std;
const int N = 1010;
int n, Q, t[7];
queue<string> prg[N];
deque<int> qr;
queue<int> qb;
map<string, int> var;
int cur, past, lockk;
int CheckNum(string str){
    for (int i = 0; i < str.size(); i++){
        if (!isdigit(str[i]) && str[i] != '-')
            return 0;
    }
    return 1;
}
void bf(){
    cur = qr.front();
    qr.pop_front();
    past = 0;
    while (past < Q && prg[cur].size()){
        string s = prg[cur].front();
        string name, tmp, v;
        if (s.find("+=") != string::npos){
            stringstream ss(s);
            ss >> name >> tmp >> v;
            if (CheckNum(v))
                var[name] += atoi(v.c_str());
            else
                var[name] += var[v];
            past += t[1];
        }
        else if (s.find("-=") != string::npos){
            stringstream ss(s);
            ss >> name >> tmp >> v;
            if (CheckNum(v))
                var[name] -= atoi(v.c_str());
            else
                var[name] -= var[v];
            past += t[2];
        }
        else if (s.find('=') != string::npos){
            stringstream ss(s);
            ss >> name >> tmp >> v;
            if (CheckNum(v))
                var[name] = atoi(v.c_str());
            else
                var[name] = var[v];
            past += t[0];
        }
        else if (s.find("print") != string::npos){
            stringstream ss(s);
            ss >> tmp >> name;
            cout << cur + 1 << ": " << var[name] << endl;
            past += t[3];
        }
        else if (s == "lock"){
            if (lockk){
                qb.push(cur);
                return;
            }
            lockk = 1;
            past += t[4];
        }
        else if (s == "unlock"){
            lockk = 0;
            if (qb.size()){
                qr.push_front(qb.front());
                qb.pop();
            }
            past += t[5];
        }
        else if (s == "end") 
            return;
        prg[cur].pop(); 
    }
    qr.push_back(cur);
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    cin >> n;
    FOR(i, 7){
        cin >> t[i];
    }
    cin >> Q;
    string s;
    FOR(i, n){
        do{
            getline(cin, s);
            prg[i].push(s);
        } while (s != "end");
        qr.push_back(i);
    }
    lockk = 0;
    while (qr.size()){
        bf();
    }
    return 0;
}