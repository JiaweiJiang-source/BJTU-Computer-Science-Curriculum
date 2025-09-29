#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include<cstring>

using namespace std;

set<string> Vt; 
set<string> Vn; 
map<string, set<string>> FirstVt; 
map<string, set<string>> LastVt;
map<string, int>VtIndex; 
vector<vector<string>> table; 

// �����ķ�����(�ع��ķ�)
map<string, vector<string>> grammar = {
    {"S", {"#E#"}},
    {"E", {"E+T","E-T","T"}},
    {"T", {"T*F","T/F","F"}},
    {"F",{"(E)","i"}}
};

// �ж��Ƿ��Ƿ��ս��
bool isVn(const string& symbol) {
    return Vn.count(symbol);
}

// �ж��Ƿ����ս��
bool isVt(const string& symbol) {
    return Vt.count(symbol);
}

// ���� FirstVT ��
map<string, set<string>> calculateFirstVtSet()
{
    map<string, set<string>> firstSet;

    // ��ʼ�����ս���� First ��
    for (map<string, vector<string>>::iterator entry = grammar.begin(); entry != grammar.end(); entry++)
    {
        firstSet[entry->first] = {};
        for (vector<string>::iterator production = entry->second.begin(); production != entry->second.end(); production++) //��ÿ���Ҳ���һ����ڶ�����ĸ���ս����ֱ�Ӽ���
        {
            string s = *production;
            if (isVt(s.substr(0, 1)))
                firstSet[entry->first].insert(s.substr(0, 1));
            else if (isVt(s.substr(1, 1)))
                firstSet[entry->first].insert(s.substr(1, 1));
        }
    }

    bool changes = true;
    while (changes)
    {
        changes = false;

        for (map<string, vector<string>>::iterator entry = grammar.begin(); entry != grammar.end(); entry++)
        {
            string nonTerminal = entry->first;
            int originalSize = firstSet[nonTerminal].size();
            for (vector<string>::iterator it = entry->second.begin(); it != entry->second.end(); it++)
            {
                string production = *it; // һ������ʽ�Ҳ�
                string vn = production.substr(0, 1);
                if (isVn(vn))// ����Ƿ��ս��
                {
                    for (set<string>::iterator fi = firstSet[vn].begin(); fi != firstSet[vn].end(); fi++)
                        firstSet[nonTerminal].insert(*fi);
                }

            }
            if (firstSet[nonTerminal].size() != originalSize) //��first�ı䣬���������
            {
                changes = true;
            }
        }

    }

    return firstSet;
}

// ���� LastVt ��
map<string, set<string>> calculateLastVtSet()
{
    map<string, set<string>> LastSet;

    // ��ʼ�����ս���� LastVt ��
    for (map<string, vector<string>>::iterator entry = grammar.begin(); entry != grammar.end(); entry++)
    {
        LastSet[entry->first] = {};
        for (vector<string>::iterator production = entry->second.begin(); production != entry->second.end(); production++) //��ÿ���Ҳ���һ����ڶ�����ĸ���ս����ֱ�Ӽ���
        {
            string s = *production;
            int len = s.length() - 1;
            if (isVt(s.substr(len, 1)))
                LastSet[entry->first].insert(s.substr(len, 1));
            else if (len > 1 && isVt(s.substr(len - 1, 1)))
                LastSet[entry->first].insert(s.substr(len - 1, 1));
        }
    }

    bool changes = true;
    while (changes)
    {
        changes = false;

        for (map<string, vector<string>>::iterator entry = grammar.begin(); entry != grammar.end(); entry++)
        {
            string nonTerminal = entry->first;
            int originalSize = LastSet[nonTerminal].size();
            for (vector<string>::iterator it = entry->second.begin(); it != entry->second.end(); it++)
            {
                string production = *it; // һ������ʽ�Ҳ�
                string vn = production.substr(production.length() - 1, 1);
                if (isVn(vn))// ����Ƿ��ս��
                {
                    for (set<string>::iterator fi = LastSet[vn].begin(); fi != LastSet[vn].end(); fi++)
                        LastSet[nonTerminal].insert(*fi);
                }

            }
            if (LastSet[nonTerminal].size() != originalSize) //��first�ı䣬���������
            {
                changes = true;
            }
        }

    }

    return LastSet;
}

// ����OPM
void parseTable()
{
    for (map<string, vector<string>>::iterator it = grammar.begin(); it != grammar.end(); it++)
    {
        for (vector<string>::iterator tmp = it->second.begin(); tmp != it->second.end(); tmp++)
        {
            string str = *tmp; // �Ҳ�
            for (int i = 0; i + 1 < str.size(); i++)
            {
                if (i + 2 < str.size())
                {
                    if (isVt(str.substr(i, 1)) && isVn(str.substr(i + 1, 1)) && isVt(str.substr(i + 2, 1)))
                        table[VtIndex[str.substr(i, 1)]][VtIndex[str.substr(i + 2, 1)]] = "=";
                }
                if (isVt(str.substr(i, 1)))
                {
                    if (isVt(str.substr(i + 1, 1)))
                        table[VtIndex[str.substr(i, 1)]][VtIndex[str.substr(i + 1, 1)]] = "=";
                    else if (isVn(str.substr(i + 1, 1)))
                    {
                        for (set<string>::iterator fi = FirstVt[str.substr(i + 1, 1)].begin(); fi != FirstVt[str.substr(i + 1, 1)].end(); fi++)
                            table[VtIndex[str.substr(i, 1)]][VtIndex[*fi]] = "<";
                    }
                }
                else if (isVn(str.substr(i, 1)))
                {
                    if (isVt(str.substr(i + 1, 1)))
                    {
                        for (set<string>::iterator la = LastVt[str.substr(i, 1)].begin(); la != LastVt[str.substr(i, 1)].end(); la++)
                            table[VtIndex[*la]][VtIndex[str.substr(i + 1, 1)]] = ">";
                    }
                }
            }
        }
    }
}

// ��ȡ��Ԫʽ�ļ�������OPT����
void analyzeExpression(const char* expressionFile)
{
    cout << "��ȡ�ļ���֪������䣺" << endl;
    FILE* fp;
    char buf[1024];
    string shizi, like;
    if ((fp = fopen(expressionFile, "r")) != NULL)
    {
        while (fgets(buf, 1024, fp) != NULL)
        {
            int len = strlen(buf);
            buf[len - 1] = '\0';  /*ȥ�����з�*/
            printf("%s \n", buf);

            if (buf[1] == '2') // ˵��Ϊ��ʶ��
            {
                like += 'i';
            }
            for (int i = 3; i < len - 2; i++)
            {
                shizi = shizi + buf[i];
                if (buf[1] != '2')
                {
                    like += buf[i];
                }
            }
        }
    }
    shizi += '#';
    like += '#';
    fclose(fp);

    cout << "��������Ϊ��" << shizi << endl;
    cout << "�������Ϊ��" << like << endl;

    cout << "OPT�������:" << endl;

    string expression = like;
    string result = "ACCEPT";
    string topOfStack = "#";

    while ((expression.size() > 0) && result == "ACCEPT")
    {
        string nextSymbol = expression.substr(0, 1);
        if (!isVt(nextSymbol) && !isVn(nextSymbol))
        {
            result = "REJECT";
            cout << "Error: undefined symbol! :\"" << nextSymbol << "\"" << endl;
            break;
        }
        if (isVt(topOfStack.substr(0, 1)))
        {
            string op = table[VtIndex[topOfStack.substr(0, 1)]][VtIndex[nextSymbol]];
            if (op == "=" || op == "<")
            {
                cout << "Push: " << nextSymbol << endl;
                expression = expression.substr(1);
                topOfStack = nextSymbol + topOfStack;
            }
            else if (op == ">")
            {
                cout << "Apply production: " << endl;
                int j = 1;
                string Q = topOfStack.substr(0, 1);
                while (isVn(topOfStack.substr(j, 1)) || (isVt(topOfStack.substr(j, 1)) && table[VtIndex[topOfStack.substr(j, 1)]][VtIndex[Q]] != "<"))
                {
                    if (isVn(topOfStack.substr(j, 1)))
                        j++;
                    else
                    {
                        Q = topOfStack.substr(j, 1);
                        j++;
                    }
                }
                topOfStack = "N" + topOfStack.substr(j);
            }
            else
            {
                result = "REJECT";
                cout << "Error: No op relationship! " << nextSymbol << endl;
            }
        }
        else
        {
            string op = table[VtIndex[topOfStack.substr(1, 1)]][VtIndex[nextSymbol]];
            if (op == "=" || op == "<")
            {
                cout << "Push: " << nextSymbol << endl;
                expression = expression.substr(1);
                topOfStack = nextSymbol + topOfStack;
            }
            else if (op == ">")
            {
                cout << "Apply production: " << endl;
                int j = 2;
                while (isVn(topOfStack.substr(j, 1)) || (isVt(topOfStack.substr(j, 1)) && table[VtIndex[topOfStack.substr(j, 1)]][VtIndex[topOfStack.substr(0, 1)]] != "<"))
                    j++;
                topOfStack = "N" + topOfStack.substr(j);
            }
            else
            {
                result = "REJECT";
                cout << "Error: No op relationship! " << nextSymbol << endl;
            }
        }
        cout << topOfStack << "\t\t\t" << expression << endl;
    }

    if (topOfStack.size() == 3 && expression.size() == 0 && result == "ACCEPT")
    {
        cout << "Accepted!" << endl;
    }
    else
    {
        cout << "Rejected!" << endl;
    }

    cout << "----------------------------------------" << endl;
}

int main()
{
    int cntVt = 0;

    // ��¼Vn���Ϻ�Vt���ϼ����ڷ������е��±�
    for (map<string, vector<string>>::iterator it = grammar.begin(); it != grammar.end(); ++it)
    {
        pair<string, vector<string>> temp = *it;

        Vn.insert(temp.first);

        vector<string> production = temp.second;
        for (vector<string>::iterator iit = production.begin(); iit != production.end(); iit++)
        {
            string ss = *iit;
            for (int j = 0; j < ss.length(); j++)
            {
                //��¼����ʽ�еķ��ս�����ս��
                if (ss[j] >= 'A' && ss[j] <= 'Z')
                { // ��д��ĸ
                    if (ss[j + 1] == '\'')
                    {          // ��'���������Ϊһ�����ս��
                        Vn.insert(ss.substr(j, 2));
                        j++;
                    }
                    else
                    {
                        Vn.insert(ss.substr(j, 1));
                    }
                }
                else
                {
                    Vt.insert(ss.substr(j, 1)); // ���ս��
                    string v = ss.substr(j, 1);
                    if (v != "" && VtIndex[v] == 0)
                        VtIndex[v] = cntVt++;
                }
            }

        }
    }

    table.resize(Vt.size(), vector<string>(Vt.size(), ""));
    Vn.insert("N");

    // ��FirstVt����
    FirstVt = calculateFirstVtSet();
    // ��LastVt����
    LastVt = calculateLastVtSet();

    // ����OPM������
    parseTable();

    // ��ȡ��Ԫʽ�ļ�������OPT����
    analyzeExpression("D:/lexical_analysis_output.txt");


    // ���Vn����
    cout << "Vn����: ";
    for (set<string>::iterator it = Vn.begin(); it != Vn.end(); it++)
        cout << *it << ' ';
    cout << endl;

    // ���Vt����
    cout << "Vt����: ";
    for (set<string>::iterator it = Vt.begin(); it != Vt.end(); it++)
        cout << *it << ' ';
    cout << endl;

    // ���FirstVt����
    cout << "FirstVt����: " << endl;
    for (map<string, set<string>>::iterator it = FirstVt.begin(); it != FirstVt.end(); it++)
    {
        cout << it->first << ": ";
        for (set<string>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            cout << '\"' << *it2 << '\"' << " ";
        }
        cout << endl;
    }

    // ���LastVt����
    cout << "LastVt����: " << endl;
    for (map<string, set<string>>::iterator it = LastVt.begin(); it != LastVt.end(); it++) {
        cout << it->first << ": ";
        for (set<string>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            cout << '\"' << *it2 << '\"' << " ";
        }
        cout << endl;
    }



    // ���OPM������
    cout << "-------------OPM������------------------" << endl;
    cout << "----------------------------------------" << endl;
    cout << "\t|";
    vector<string> vtt;
    for (set<string>::iterator vt = Vt.begin(); vt != Vt.end(); vt++)
    {
        cout << *vt << "\t|";
        vtt.push_back(*vt);
    }
    cout << endl;
    for (set<string>::iterator vt = Vt.begin(); vt != Vt.end(); vt++)
    {
        cout << *vt << "\t|";
        for (int j = 0; j < Vt.size(); j++)
        {
            cout << table[VtIndex[*vt]][VtIndex[vtt[j]]] << "\t|";
        }
        cout << endl;
    }



    return 0;

}