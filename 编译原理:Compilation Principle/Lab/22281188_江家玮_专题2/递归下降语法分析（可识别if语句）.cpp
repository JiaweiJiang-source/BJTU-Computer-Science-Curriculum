#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include <cassert>
#include<fstream>
#define MAX 1024
using namespace std;

char s[500];
int i;
int SIGN;//  用来标记语句是否正确

void S();
void E();
void E_();
void T();
void T_();
void F();
void A();
void M();
void V();
void IF();

void S()
{
	if (SIGN != 0) return;

	printf("检查 S 中...\n");

	// 检查是否是 'if' 语句
	if (strncmp(&s[i], "if", 2) == 0) {
		IF(); // 调用 IF 解析函数
		return; // 解析完 if 语句直接返回
	}

	// 检查是否是变量赋值
	if (s[i] == 'i') {
		V(); // 处理变量的逻辑
	}
	else {
		SIGN = 1;
		cout << "错误：在 S 处缺少变量" << endl;
		return;
	}

	if (SIGN == 0 && s[i] == '=') {
		i++; // 跳过 '='
		E(); // 解析等号后的表达式
	}
	else {
		SIGN = 1;
		cout << "错误：在 S 处缺少'='" << endl;
	}
}

void IF() {
	if (SIGN != 0) return;

	// 检查是否是 'if' 关键字
	if (strncmp(&s[i], "if", 2) == 0) {
		i += 2; // 跳过 'if'

		// 检查 '(' 开始条件表达式
		if (s[i] == '(') {
			i++; // 跳过 '('
			E(); // 解析条件表达式

			// 检查 ')' 结束条件表达式
			if (SIGN == 0 && s[i] == ')') {
				i++; // 跳过 ')'

				// 检查 '{' 开始 if 语句块
				if (s[i] == '{') {
					i++; // 跳过 '{'
					S(); // 解析 if 语句块

					// 检查 '}' 结束 if 语句块
					if (SIGN == 0 && s[i] == '}') {
						i++; // 跳过 '}'

						// 检查是否有 'else' 语句
						if (strncmp(&s[i], "else", 4) == 0) {
							i += 4; // 跳过 'else'

							// 检查 '{' 开始 else 语句块
							if (s[i] == '{') {
								i++; // 跳过 '{'
								S(); // 解析 else 语句块

								// 检查 '}' 结束 else 语句块
								if (SIGN == 0 && s[i] == '}') {
									i++; // 跳过 '}'
								}
								else {
									SIGN = 1;
									cout << "错误：在 else 中缺少右大括号 '}'" << endl;
								}
							}
							else {
								SIGN = 1;
								cout << "错误：在 else 中缺少左大括号 '{'" << endl;
							}
						}
					}
					else {
						SIGN = 1;
						cout << "错误：在 if 语句中缺少右大括号 '}'" << endl;
					}
				}
				else {
					SIGN = 1;
					cout << "错误：在 if 语句中缺少左大括号 '{'" << endl;
				}
			}
			else {
				SIGN = 1;
				cout << "错误：在 if 语句中缺少右括号 ')'" << endl;
			}
		}
		else {
			SIGN = 1;
			cout << "错误：在 if 语句中缺少左括号 '('" << endl;
		}
	}
	else {
		SIGN = 1;
		cout << "错误：在 if 语句中缺少 'if' 关键字" << endl;
	}
}





void E() {
	if (SIGN == 0) {
		if (s[i] == '(' || s[i] == 'i') {
			T();
			if (SIGN == 0) {
				if (s[i] == '+' || s[i] == '-') {
					E_(); // 解析更多的加减操作
				}
				else if (s[i] == ')' || s[i] == '#') {
					return; // 合法结束
				}
				else {
					SIGN = 1;
					cout << "E错误：遇到无效字符" << endl;
				}
			}
		}
		else {
			SIGN = 1;
			cout << "E错误：表达式应以'('或变量开始" << endl;
		}
	}
}


void E_() {
	if (SIGN != 0) return;

	// 如果遇到 '+' 或 '-' 操作符，解析下一个表达式
	while (s[i] == '+' || s[i] == '-') {
		i++; // 跳过操作符
		T(); // 解析下一个项
		if (SIGN != 0) return; // 如果解析失败，立即返回
	}

	// 合法的终止符应该是右括号 ')' 或结束符 '#'
	if (s[i] != ')' && s[i] != '#' && s[i] != '}') {
		SIGN = 1;
		cout << "错误：在 E' 中遇到无效字符" << endl;
	}
}


void T() {
	if (SIGN != 0) return;

	// 检查因子是否是 '(' 或 'i'
	if (s[i] == '(' || s[i] == 'i') {
		F(); // 解析因子
		if (SIGN != 0) return;

		// 处理乘除操作符
		while (s[i] == '*' || s[i] == '/') {
			T_(); // 解析乘除
			if (SIGN != 0) return;
		}
	}
	else {
		SIGN = 1;
		cout << "错误：在 T 中遇到无效字符，缺少因子" << endl;
	}
}


void T_()
{
	// 先检查是否有语法错误
	if (SIGN != 0) return;
	printf("T'   ");

	// 判断当前字符是否为 '*' 或 '/'（表示有后续的乘除操作）
	while (s[i] == '*' || s[i] == '/') {
		M(); // 解析乘除操作符
		if (SIGN != 0) return; // 如果解析失败则返回

		// 解析接下来的因子（F）
		if (s[i] == '(' || s[i] == 'i') {
			F(); // 解析因子
		}
		else {
			SIGN = 1;
			cout << "错误：在 T' 中缺少有效的因子（F）" << endl;
			return;
		}

		// 检查是否继续处理连续的 '*' 或 '/' 运算符
		if (SIGN != 0) return;
	}

	// 检查 T' 的结束条件，应该为 '+'、'-'、')' 或 '#'
	if (s[i] != '+' && s[i] != '-' && s[i] != ')' && s[i] != '#') {
		SIGN = 1;
		cout << "错误：在 T' 中遇到无效字符" << endl;
	}
}


void F()
{
	// 先检查是否有语法错误
	if (SIGN != 0) return;
	printf("F   ");

	// 判断当前字符是否为 '('，表示可能是一个括号内的表达式
	if (s[i] == '(') {
		i++; // 跳过 '('

		// 解析括号内的表达式
		if (s[i] == '(' || s[i] == 'i') {
			E(); // 调用 E 解析括号内的内容
		}
		else {
			SIGN = 1;
			cout << "错误：在 F 中缺少有效的表达式" << endl;
			return;
		}

		// 检查是否有匹配的 ')'
		if (SIGN == 0 && s[i] == ')') {
			i++; // 跳过 ')'
		}
		else {
			SIGN = 1;
			cout << "错误：在 F 中缺少右括号 ')'" << endl;
			return;
		}
	}
	// 如果当前字符是 'i'，表示一个变量
	else if (s[i] == 'i') {
		i++; // 跳过变量 'i'
	}
	else {
		// 当前字符既不是 '(' 也不是 'i'，标记错误
		SIGN = 1;
		cout << "错误：在 F 中遇到无效字符" << endl;
	}
}


void A()
{
	if (SIGN == 0) {
		printf("A   ");
		if (s[i] == '+' || s[i] == '-') {
			i++;
		}
		else {
			SIGN = 1;
			cout << "A错误" << endl;
		}
	}
}

void M()
{
	if (SIGN == 0) {
		printf("M   ");
		if (s[i] == '*' || s[i] == '/') {
			i++;
		}
		else {
			SIGN = 1;
			cout << "M错误" << endl;
		}
	}
}

void V()
{
	if (SIGN == 0) {
		printf("V   ");
		if (s[i] == 'i') {
			i++;
		}
		else {
			SIGN = 1;
			cout << "V错误" << endl;
		}
	}
}

int main()
{
	FILE* fp;
	string str, med;
	char arr[MAX];

	//printf("输入语句为：\n");
	if ((fp = fopen("C://Users/37623/Desktop/lexical_analysis_output.txt", "r")) == NULL)
	{
		printf("ERROE!\n");
		return 0;
	}

	if ((fp = fopen("C://Users/37623/Desktop/lexical_analysis_output.txt", "r")) != NULL) {
		while (fgets(arr, MAX, fp) != NULL)
		{
			int len = strlen(arr);
			arr[len - 1] = '\0';
			//printf("%s \n", arr);
			int flag = 0;//标记用于引号检测
			if (arr[1] == '2') //读到变量时
			{
				med += 'i';

			}

			for (int i = 0; i < len; i++)
			{
				if (arr[i] == ',' && flag == 0) //找到逗号
				{
					i++;
					while (arr[i] != '\0') {
						str += arr[i];
						if (arr[1] != '2') {
							med += arr[i];
						}
						if (arr[i + 2] == '\0')//最后一个字符
							break;
						i++;
					}
				}
			}
		}
	}
	str += '#';
	med += '#';
	fclose(fp);

	cout << "输入的语句为：" << str << endl;
	cout << "转化为：" << med << endl;

	SIGN = 0;
	i = 0;

	// 将 like 转换为字符数组 s
	strncpy(s, med.c_str(), sizeof(s) - 1); // 使用 strncpy 进行安全复制
	s[sizeof(s) - 1] = '\0'; // 确保字符串以 '\0' 结尾

	// 检查输入是否只有结束符 '#'
	if (s[0] == '#') {
		return 0;
	}

	// 调用解析函数 S 进行语法分析
	S();

	// 检查分析结果，确定语句是否合法
	if (s[i] == '#' && SIGN == 0) {
		printf("\n语句合法\n");
	}
	else {
		printf("\n语句不合法\n");
	}

	return 1;
}

