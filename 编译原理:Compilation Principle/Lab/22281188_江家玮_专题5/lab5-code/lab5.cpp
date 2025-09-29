#include "main.h"

void split() {
    int position = 0;
    for (set<string>::iterator it = prod.begin(); it != prod.end(); it++) {
        string temp = *it;
        for (int i = 0; i < temp.length(); i++) {
            position = temp.find("->");
            string s = temp.substr(0, position);
            string ss = temp.substr(position + 2);
            set<string>sss;
            string t;
            for (int j = 0; j < ss.length(); j++) {
                if (ss[j] == '|') {
                    sss.insert(t);
                    t = "";
                }
                else  t.append(ss.substr(j, 1));
            }
            sss.insert(t);
            splProd.insert(pair<string, set<string> >(s, sss));
        }
    }

}

bool isNumber(const string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

//从文件读取&分解产生式
void readIn() {
    string line;
    ifstream in("lexical.txt");

    if (in) {
        getline(in, line);
        prod.insert(line);
        start = line.substr(0, 1);
        firstItem.prod = line;
        follow[line.substr(0, 1)].insert("#");//#加入起始符的FOLLOW集
        while (getline(in, line)) prod.insert(line);
    }
}
//获得终结符和非终结符
void VtVn() {
    for (set<string>::iterator it = prod.begin(); it != prod.end(); it++) {
        string temp = *it;
        for (int i = 0; i < temp.length(); i++) {
            if ((temp[i] == '-' && temp[i + 1] == '>') || temp[i] == '|' || temp[i] == '>')  continue;
            if (temp[i] >= 'A' && temp[i] <= 'Z') {//大写字母
                if (temp[i + 1] == '\'') {//有'则读入俩作为一个非终结符
                    Vn.insert(temp.substr(i, 2));
                    i++;
                }
                else  Vn.insert(temp.substr(i, 1));
            }
            else  Vt.insert(temp.substr(i, 1));//是终结符
        }
    }
    VtPlus = Vt;
    VtPlus.insert("#");
}
bool isVn(string s) {
    return Vn.find(s) != Vn.end();
}
bool isVt(string s) {
    return VtPlus.find(s) != VtPlus.end();
}
//获得单个非终结符first集
set<string> OneFirst(string s) {
    if (splProd.count(s) > 0) {
        set<string>temp = splProd[s];
        for (set<string>::iterator it = temp.begin(); it != temp.end(); it++) {
            string prodRight = *it;
            for (int i = 0; i < prodRight.length(); i++) {
                int flag = 0;//当前的非终结符的first集有^标识
                //不带’非终结符
                if (isVn(prodRight.substr(i, 1))) {
                    string t = prodRight.substr(i, 1);
                    if (t == s) continue;
                    set<string>t2 = OneFirst(prodRight.substr(i, 1));
                    for (set<string>::iterator ii = t2.begin(); ii != t2.end(); ii++) {
                        first[s].insert(*ii);
                    }
                }
                else  first[s].insert(prodRight.substr(i, 1)); //终结符
                if (flag == 0)  break;
            }
        }
    }
    return first[s];
}
//获得所有非终结符First集
void getFirst() {
    for (map<string, set<string> >::iterator it = splProd.begin(); it != splProd.end(); it++) {
        OneFirst(it->first);
    }
    cout << "First：" << endl;
    for (map<string, set<string> >::iterator it = first.begin(); it != first.end(); it++) {
        cout << "     ";
        cout << it->first << ":";
        for (set<string>::iterator ii = it->second.begin(); ii != it->second.end(); ii++) {
            cout << *ii << "  ";
        }
        cout << endl;
    }

}
//FOLLOW集的步骤3反复
void repeat() {
    for (map<string, set<string> >::iterator it = splProd.begin(); it != splProd.end(); it++) {
        string left = it->first;
        set<string>right = it->second;
        for (set<string>::iterator ii = right.begin(); ii != right.end(); ii++) {
            string temp = *ii;
            for (int j = temp.length() - 1; j > 0; j--) {
                string now;
                now = temp.substr(j, 1);
                if (isVt(now))  break;//产生式最后是终结符
                else {//产生式最后是非终结符
                    set<string>aa = follow[left];
                    for (set<string>::iterator pp = aa.begin(); pp != aa.end(); pp++) {
                        follow[now].insert(*pp);
                    }
                }
            }
        }
    }
}
//获得所有非终结符follow集
void getFollow() {
    for (map<string, set<string> >::iterator it = splProd.begin(); it != splProd.end(); it++) {
        string left = it->first;
        set<string>right = it->second;
        for (set<string>::iterator ii = right.begin(); ii != right.end(); ii++) {
            string prodRight = *ii;
            for (int i = 0; i < prodRight.length(); i++) {
                if (isVt(prodRight.substr(i, 1)))  continue;//终结符跳过
                else {//不带’非终结符
                    if (i + 1 < prodRight.length() && isVt(prodRight.substr(i + 1, 1))) {//非终结符+终结符
                        follow[prodRight.substr(i, 1)].insert(prodRight.substr(i + 1, 1));//终结符first集加入follow集中
                        i++;
                    }
                    else {//非终结符+非终结符N
                        string N;
                        N = prodRight.substr(i + 1, 1);
                        //A->aB
                        if (N == "") {
                            set<string> temp = follow[left];
                            for (set<string>::iterator nn = temp.begin(); nn != temp.end(); nn++) {
                                follow[prodRight.substr(i, 1)].insert(*nn);//FOLLOW(A)加入FOLLOW(B)
                            }
                        }
                        else {
                            set<string> ff = first[N];
                            for (set<string>::iterator nn = ff.begin(); nn != ff.end(); nn++) {
                                follow[prodRight.substr(i, 1)].insert(*nn);//把非终结符N的first集加入follow集中
                            }
                        }
                    }
                }
            }
        }
    }
    int i = 5;//多重复几次，确保FOLLOW正确
    while (i--)  repeat();
    cout << "Follow：" << endl;
    for (map<string, set<string> >::iterator it = follow.begin(); it != follow.end(); it++) {
        cout << "     ";
        cout << it->first << ":";
        for (set<string>::iterator ii = it->second.begin(); ii != it->second.end(); ii++) {
            cout << *ii << "  ";
        }
        cout << endl;
    }
}

int cDif(status m, status n) { //两个状态是否相等
    if (m.Itemlist.size() != n.Itemlist.size()) return 1;
    else {
        for (list<Item>::iterator i = m.Itemlist.begin(), j = n.Itemlist.begin(); i != m.Itemlist.end() || j != n.Itemlist.end(); i++, j++) {
            if (i->prod != j->prod || i->pos != j->pos)  //判断是否相等
                return 1; //任一个不同则不等
        }
    }
    return 0;
}
status getClosure(status& dfa) {
    for (list<Item>::iterator iter = dfa.Itemlist.begin(); iter != dfa.Itemlist.end(); iter++) {
        string jud = iter->prod.c_str();
        jud = jud.substr(iter->pos, 1);//·后第一个符号
        if (isVn(jud)) {//·后第一个是Vn则
            for (map<string, set<string> >::iterator it = splProd.begin(); it != splProd.end(); it++) {
                if (jud == it->first) {//找到对应Vn的产生式
                    for (set<string>::iterator ii = it->second.begin(); ii != it->second.end(); ii++) {
                        //Vn产生式的·后一个是Vn
                        if (isVn((*ii).substr(0, 1))) {
                            for (map<string, set<string> >::iterator itt = splProd.begin(); itt != splProd.end(); itt++) {
                                if ((*ii).substr(0, 1) == itt->first) {//找到对应Vn的产生式
                                    for (set<string>::iterator iii = itt->second.begin(); iii != itt->second.end(); iii++) {
                                        string prod = itt->first + "->" + *iii;
                                        int q = 0;
                                        Item x;
                                        x.prod = prod;
                                        for (list<Item>::iterator it = dfa.Itemlist.begin(); it != dfa.Itemlist.end(); it++) {//在dfa中找是否有重复
                                            if ((it->prod == x.prod) && (it->pos == x.pos)) {
                                                q = 1;
                                                break;
                                            }
                                        }
                                        if (q == 0)  dfa.Itemlist.push_back(x);
                                    }
                                }
                            }
                        }
                        string prod = it->first + "->" + *ii;
                        int q = 0;
                        Item x;
                        x.prod = prod;
                        for (list<Item>::iterator it = dfa.Itemlist.begin(); it != dfa.Itemlist.end(); it++) {
                            if ((it->prod == x.prod) && (it->pos == x.pos)) {
                                q = 1;
                                break;
                            }
                        }
                        if (q == 0)  dfa.Itemlist.push_back(x);
                    }
                }
            }
        }
    }
    return dfa;
}

int getProdNum(string jud) {
    int rec = 0, flag = -1;
    string s = jud.substr(3);
    for (map<string, set<string> >::iterator it = splProd.begin(); it != splProd.end(); it++) {
        if (jud.substr(0, 1) == it->first) {
            for (set<string>::iterator ii = it->second.begin(); ii != it->second.end(); ii++) {
                if (s == *ii) {
                    flag = 1;
                    break;
                }
                else rec++;
            }
        }
        if (flag == 1) break;
        for (int i = it->second.size(); i > 0; i--) {
            rec++;
        }
    }
    return rec;
}
void createSLRTable() {
    int numItem = 0;
    status L, temp;
    L.number = 0;
    L.Itemlist.push_back(firstItem);
    DFA.push_back(L); //DFA初始加入S->A
    list<status>::iterator dfa;
    for (dfa = DFA.begin(); dfa != DFA.end(); dfa++) {
        list<Item>::iterator iter;
        getClosure(*dfa); //求出I0闭包
        for (iter = dfa->Itemlist.begin(); iter != dfa->Itemlist.end(); iter++) {//对状态中的每个产生式右部进行处理
            if (iter->finish != -1) {//尚未完成
                status newItem;  //新状态
                newItem.number = DFA.size();
                if (iter->pos < iter->prod.length()) {//不是规约项目
                    string jud = iter->prod.c_str();
                    jud = jud.substr(iter->pos, 1);//·后第一个符号
                    if (isVn(jud)) { //·后第一个是Vn则
                        GOTO.push_back({ dfa->number,jud, newItem.number });
                    }
                    else ACTION.push_back({ dfa->number,jud,"s", newItem.number });//移进
                    for (list<Item>::iterator i = iter; i != dfa->Itemlist.end(); i++) {
                        Item s;
                        s.prod = iter->prod;
                        if (i->prod[i->pos] == iter->prod[iter->pos]) {//·后若一样
                            s.prod = i->prod;
                            s.pos = i->pos + 1;
                            if (s.pos == i->prod.length())
                                s.finish = 1;
                            newItem.Itemlist.push_back(s);
                            //rec++;
                            i->finish = -1;
                        }
                    }
                    int judge = -1, count = 0;
                    for (list<status>::iterator ii = DFA.begin(); ii != DFA.end(); ii++, count++) {//判断是否有重复
                        judge = cDif(*ii, getClosure(newItem));
                        if (judge == 0) break;
                    }
                    if (judge == 0) {//重复的
                        if (isVn(jud)) {//放到重复的集和中
                            GOTO.pop_back();
                            GOTO.push_back({ dfa->number,jud,count });
                        }
                        else {//Action 集
                            ACTION.pop_back();
                            ACTION.push_back({ dfa->number,jud,"s",count });
                        }
                    }
                    if (judge == 1) DFA.push_back(newItem);
                }
                else {//已经完成规约
                    int rec = 0;
                    string jud = iter->prod.c_str();
                    rec = getProdNum(jud);
                    string right = jud.substr(3);
                    if (right.find('=') != -1 || right.find('+') != -1 || right.find('-') != -1 || right.find('*') != -1 || right.find('/') != -1) {
                        quaterI.push_back(rec);
                    }
                    jud = jud.substr(0, 1);//第一个符号 产生式左侧
                    for (map<string, set<string> >::iterator it = follow.begin(); it != follow.end(); it++) {
                        if (it->first == jud) {
                            for (set<string>::iterator ii = it->second.begin(); ii != it->second.end(); ii++) {
                                ACTION.push_back({ dfa->number,*ii,"r",rec });  //对应的vt位置写入r 第rec条产生式
                            }
                        }
                    }
                }
            }
            if (iter->finish == -1) {
                if (iter->pos == iter->prod.length()) iter->finish = 1;
                else iter->finish = 0;
            }
        }
    }
    //打印项目族
    numItem = 0;
    cout << "getClosure:" << endl;
    for (dfa = DFA.begin(); dfa != DFA.end(); dfa++) {
        cout << "C" << numItem++ << "={ ";
        for (list<Item>::iterator iii = dfa->Itemlist.begin(); iii != dfa->Itemlist.end(); iii++) {
            if (iii == dfa->Itemlist.begin()) {}
            else cout << ", ";
            Item s = *iii;
            int r;
            for (r = 0; r < s.prod.length(); r++) {
                if (r == s.pos) cout << "." << s.prod[r];
                else cout << s.prod[r];
            }
            if (r == s.pos) cout << ".";
        }
        cout << " }" << endl;
    }
    cout << endl;
}

void checkConflict() {
    int reduce, shift, conflict = -1;
    list<status>::iterator dfa;
    cout << "含有冲突：";
    for (dfa = DFA.begin(); dfa != DFA.end(); dfa++) {
        reduce = 0;
        shift = 0;
        set<string> shiftrec;
        map<string, set<string> > followrec;//follow集
        for (list<Item>::iterator iter = dfa->Itemlist.begin(); iter != dfa->Itemlist.end(); iter++) {//每个状态的每条Item
            if (iter->finish == 1) {//是规约项目
                reduce++;
                continue;
            }
            string jud = iter->prod.c_str();
            jud = jud.substr(iter->pos, 1);//·后第一个符号
            //是移进项目
            if (iter->finish == 0 && isVt(jud)) {
                shift = 1;
                shiftrec.insert(jud);
            }
        }
        if (reduce > 1 || (reduce != 0 && shift == 1)) {
            cout << "I" << dfa->number << "有冲突：";
            conflict = 0;
            for (list<Item>::iterator iter = dfa->Itemlist.begin(); iter != dfa->Itemlist.end(); iter++) {//每个状态的每条Item
                if (iter->finish == 1) {
                    for (map<string, set<string> >::iterator it = follow.begin(); it != follow.end(); it++) {
                        if (iter->prod.substr(0, 1) == it->first) {//follow集
                            for (set<string>::iterator ii = it->second.begin(); ii != it->second.end(); ii++) {
                                followrec[iter->prod.substr(0, 1)].insert(*ii);
                            }
                        }
                    }
                }
            }
        }
        if (reduce > 1) {
            for (map<string, set<string> >::iterator it = followrec.begin(), itt = ++followrec.begin();
                it != followrec.end() || itt != followrec.end(); it++, itt++) {
                set<string> jiao;
                set_intersection(it->second.begin(), it->second.end(), itt->second.begin(), itt->second.end(),
                    inserter(jiao, jiao.begin()));
                if (jiao.size() != 0) {
                    cout << "冲突不可解决！" << endl;
                    conflict = 1;
                }

            }
        }
        if (reduce != 0 && shift == 1) {
            set<string> jiao2;
            set_intersection(followrec.begin()->second.begin(), followrec.begin()->second.end(), shiftrec.begin(), shiftrec.end(), inserter(jiao2, jiao2.begin()));
            if (jiao2.size() != 0) {
                cout << "冲突不可解决" << endl;
                conflict = 1;
            }

        }
    }

    cout << endl;
}

ActionNode getAction(int I_Now, string vt) {
    for (list<ActionNode>::iterator it = ACTION.begin(); it != ACTION.end(); it++) {
        if (it->I == I_Now && it->Vt == vt) return *it;
    }
    return { 0,"","",0 };  //ERROR
}
int getGoto(int I_Now, string vn) {
    for (list<GotoNode>::iterator it = GOTO.begin(); it != GOTO.end(); it++) {
        if (it->I == I_Now && it->Vn == vn) return  it->next_I;
    }
    return -1;  //ERROR
}
//打印分析表
void SLRTable() {
    cout << "SLR(1)分析表如下" << endl;
    cout << "\t|\t\t\tAction\t\t\t\t\t\t|\t\tGOTO\t" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "I\t";
    set<string>::iterator in = VtPlus.begin();
    in++;
    for (; in != VtPlus.end(); in++)
        cout << "|" << *in << "\t";
    in = VtPlus.begin();
    cout << "|" << *in << "\t";
    for (set<string>::iterator in = Vn.begin(); in != Vn.end(); in++) {
        if (*in == "S") continue;
        cout << "|" << *in << "\t";
    }

    for (int ItemNum = 0; ItemNum < DFA.size(); ItemNum++) {
        cout << ItemNum << "\t";
        in = VtPlus.begin();
        in++;
        for (; in != VtPlus.end(); in++) {
            if (getAction(ItemNum, *in).act == "r" && getAction(ItemNum, *in).nextI_P == 6) cout << "|acc\t";
            else if (getAction(ItemNum, *in).act != "")  cout << "|" << getAction(ItemNum, *in).act << getAction(
                ItemNum, *in).nextI_P << "\t";
            else cout << "|\t";
        }
        set<string>::iterator in = VtPlus.begin();
        if (getAction(ItemNum, *in).act == "r" && getAction(ItemNum, *in).nextI_P == 6) cout << "|acc\t";
        else if (getAction(ItemNum, *in).act != "")  cout << "|" << getAction(ItemNum, *in).act << getAction(
            ItemNum, *in).nextI_P << "\t";
        else cout << "|\t";

        for (set<string>::iterator in = Vn.begin(); in != Vn.end(); in++) {
            if (*in == "S") continue;
            if (getGoto(ItemNum, *in) != -1)  cout << "|" << getGoto(ItemNum, *in) << "\t";
            else cout << "|\t";
        }
        cout << endl;
    }
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
}

//读取下一个词
bool nextWord(FILE** fp) {
    RecValue = "";
    char buf[32];
    memset(buf, 0, sizeof(buf));//初始化单词置为0
    if (!feof(*fp))  fgets(buf, 32, *fp);
    else  return false;
    string word = buf;
    if (!word.compare(""))  return false;
    string typevalue;
    typevalue = word.substr(word.find('(') + 1, word.find(',') - word.find('(') - 1);
    string value;
    value = word.substr(word.find('(') + 1, word.find(',') - word.find('(') - 1);
    if (value == " ") {
        cout << "" + typevalue;
        if (typevalue == "if") ptr = "y";
        else if (typevalue == "else") ptr = "e";
        else ptr = typevalue;
    }
    else {
        cout << value;
        ptr = value;
    }
    if (typevalue == "identifier") {
        ptr = "i";
        RecValue = value;
    }
    if (typevalue == "const")  ptr = "n";
    return true;
}
//获取二元式并转换为字符
void getOutput() {
    FILE* fp;
    fp = fopen("in.txt", "r+");  //读文件
    cout << endl;
    cout << "表达式： ";
    while (nextWord(&fp)) {
        temp.push_back(ptr);
        if (RecValue != "") recValue.push_back(RecValue);
        else recValue.push_back(ptr);
    }
    temp.push_back("#");
    recValue.push_back("#");
    //倒序push进left
    for (vector<string>::reverse_iterator it = temp.rbegin(); it != temp.rend(); it++) {
        leftString.push_back(*it);
    }
    for (vector<string>::reverse_iterator it = recValue.rbegin(); it != recValue.rend(); it++) {
        recleftValue.push_back(*it);
    }
    cout << endl;
}
string newResult() {
    char t = '1';
    t = t + recTemp;
    string Temp = "T";
    Temp = Temp.append(1, t);
    recTemp++;
    return Temp;
}

//分析输入串
void Analysis() {
    string Q, R, Vn;
    int equal = 0;
    getOutput();
    cout << endl;
    cout << "SLR(1)分析过程如下" << endl;
    cout << "|状态栈" << setw(34) << "|符号栈" << setw(35) << "|剩余串" << setw(31) << "|操作" << setw(26) << "|四元式" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------------------" << endl;
    stack.push_back({ 0, "#" });
    while (!stack.empty()) {
        string outX = "";//分析栈符号内容
        int S_top = stack.size();
        cout << "|";
        string res;
        stringstream ss;
        for (int i = 0; i < S_top; i++) ss << "s" << stack[i].S;
        ss >> res;

        cout << std::left << setw(30) << res << "|";
        for (int i = 0; i < S_top; i++) outX += stack[i].X.X;
        cout << std::left << setw(31) << outX << "|";
        vector<string>::iterator it = temp.begin();
        string left;
        stringstream ll;
        for (it; it != temp.end(); it++) ll << *it;
        ll >> left;
        string Xnext = "";
        int Snext = -1;
        cout << std::left << setw(30) << left << "|";
        vector<string>::iterator itt = temp.begin(); //*itt输入字符
        vector<string>::iterator ittv = recValue.begin(); //*ittv输入字符
        if (getAction(stack[S_top - 1].S, *itt).act == "s") {
            cout << getAction(stack[S_top - 1].S, *itt).act << getAction(stack[S_top - 1].S, *itt).nextI_P << endl;
            stack.push_back({ getAction(stack[S_top - 1].S, *itt).nextI_P, {*itt, *ittv} });
            leftString.pop_back();
            recleftValue.pop_back();
            temp.clear();//i的表达式清空
            recValue.clear();//真实value表达式清空
            for (vector<string>::reverse_iterator is = leftString.rbegin(); is != leftString.rend(); is++)
                temp.push_back(*is);
            for (vector<string>::reverse_iterator iss = recleftValue.rbegin(); iss != recleftValue.rend(); iss++)
                recValue.push_back(*iss);
            continue;
        }
        else if (getAction(stack[S_top - 1].S, *itt).act == "r") {
            string arg1, arg2, result;
            if (getAction(stack[S_top - 1].S, *itt).act == "r" && getAction(stack[S_top - 1].S, *itt).nextI_P == 6) {
                cout << "acc\t" << endl;
                break;
            }
            else {
                cout << getAction(stack[S_top - 1].S, *itt).act << getAction(stack[S_top - 1].S, *itt).nextI_P;
                int rec = getAction(stack[S_top - 1].S, *itt).nextI_P;
                int flag = 0;
                int recRight = 0;
                recRight = rec;
                for (map<string, set<string> >::iterator it = splProd.begin(); it != splProd.end(); it++) {
                    for (set<string>::iterator ii = it->second.begin(); ii != it->second.end(); ii++) {
                        if (recRight != 0) {
                            recRight--;
                            continue;//找到rec号产生式右侧*ii
                        }//rec属于qua时再找arg1、2
                        equal = 0;
                        for (int i = 0; i < (*ii).length(); i++) {
                            if (i == 0)
                                arg2 = stack[S_top - 1].X.value;
                            if (i == 1) {//arg2
                                if (stack[S_top - 1].X.value == "=") equal = 1;
                            }
                            if (i == 2) {
                                if (equal == 1) arg1 = arg2;
                                else if (stack[S_top - 1].X.value == "(" || stack[S_top - 1].X.value == ")") {
                                    list<quaterExp>::reverse_iterator s = Exp.rbegin();
                                    arg1 = s->result;
                                }
                                else arg1 = stack[S_top - 1].X.value;
                            }
                            if (stack[S_top - 1].X.value == "(" || stack[S_top - 1].X.value == ")") {}
                            else result = stack[S_top - 1].X.value;
                            stack.pop_back();
                            S_top--;
                        }
                        Xnext = it->first;
                        Snext = getGoto(stack[S_top - 1].S, Xnext);
                        flag = 1;
                        break;
                    }
                    if (flag == 1) break;
                }
                flag = 0;
                for (list<int>::iterator ii = quaterI.begin(); ii != quaterI.end(); ii++) {//生成四元式
                    if (rec == *ii) {//序号等于R项目使用的产生式序号
                        if (equal == 1) arg2 = "NULL";
                        else result = newResult();
                        int recPro = rec;
                        for (map<string, set<string> >::iterator itt = splProd.begin(); itt != splProd.end(); itt++) {
                            for (set<string>::iterator iii = itt->second.begin(); iii != itt->second.end(); iii++) {
                                if (recPro > 0) {
                                    recPro--;
                                    continue;//找到对应序号产生式的右部*iii
                                }
                                //截取出操作符
                                Exp.push_back({ (*iii).substr(1, 1), arg1, arg2, result });
                                cout << std::right << setw(21) << '(' << (*iii).substr(1, 1) << "," << arg1 << "," << arg2 << ',' << result << ')' << endl;
                                flag = 1;
                                break;
                            }
                            if (flag == 1) break;
                        }
                    }
                }
                if (flag != 1) cout << endl;
                stack.push_back({ Snext, {Xnext,result} });
                continue;
            }
        }
        else {
            cout << "Error!" << endl;
            break;
        }
    }
    cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
}
void ExpOutput() {
    FILE* fp;
    bool already_write = false;
    if (!already_write) {
        already_write = true;
        fp = fopen("out.txt", "w+");
    }
    else fp = fopen("out.txt", "a+");
    for (list<quaterExp>::iterator s = Exp.begin(); s != Exp.end(); s++) {
        string siyuanshi;
        siyuanshi = "(" + s->op + "," + s->arg1 + "," + s->arg2 + ',' + s->result + ")\n";
        fputs(siyuanshi.c_str(), fp);
    }
    fclose(fp);
}
int main() {
    readIn();
    split();
    VtVn();
    getFirst();
    getFollow();
    createSLRTable();
    checkConflict();
    SLRTable();
    Analysis();
    ExpOutput();
    return 0;
}
