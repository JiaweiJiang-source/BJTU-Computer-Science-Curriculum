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
int SIGN;//  �����������Ƿ���ȷ

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

	printf("��� S ��...\n");

	// ����Ƿ��� 'if' ���
	if (strncmp(&s[i], "if", 2) == 0) {
		IF(); // ���� IF ��������
		return; // ������ if ���ֱ�ӷ���
	}

	// ����Ƿ��Ǳ�����ֵ
	if (s[i] == 'i') {
		V(); // ����������߼�
	}
	else {
		SIGN = 1;
		cout << "������ S ��ȱ�ٱ���" << endl;
		return;
	}

	if (SIGN == 0 && s[i] == '=') {
		i++; // ���� '='
		E(); // �����Ⱥź�ı��ʽ
	}
	else {
		SIGN = 1;
		cout << "������ S ��ȱ��'='" << endl;
	}
}

void IF() {
	if (SIGN != 0) return;

	// ����Ƿ��� 'if' �ؼ���
	if (strncmp(&s[i], "if", 2) == 0) {
		i += 2; // ���� 'if'

		// ��� '(' ��ʼ�������ʽ
		if (s[i] == '(') {
			i++; // ���� '('
			E(); // �����������ʽ

			// ��� ')' �����������ʽ
			if (SIGN == 0 && s[i] == ')') {
				i++; // ���� ')'

				// ��� '{' ��ʼ if ����
				if (s[i] == '{') {
					i++; // ���� '{'
					S(); // ���� if ����

					// ��� '}' ���� if ����
					if (SIGN == 0 && s[i] == '}') {
						i++; // ���� '}'

						// ����Ƿ��� 'else' ���
						if (strncmp(&s[i], "else", 4) == 0) {
							i += 4; // ���� 'else'

							// ��� '{' ��ʼ else ����
							if (s[i] == '{') {
								i++; // ���� '{'
								S(); // ���� else ����

								// ��� '}' ���� else ����
								if (SIGN == 0 && s[i] == '}') {
									i++; // ���� '}'
								}
								else {
									SIGN = 1;
									cout << "������ else ��ȱ���Ҵ����� '}'" << endl;
								}
							}
							else {
								SIGN = 1;
								cout << "������ else ��ȱ��������� '{'" << endl;
							}
						}
					}
					else {
						SIGN = 1;
						cout << "������ if �����ȱ���Ҵ����� '}'" << endl;
					}
				}
				else {
					SIGN = 1;
					cout << "������ if �����ȱ��������� '{'" << endl;
				}
			}
			else {
				SIGN = 1;
				cout << "������ if �����ȱ�������� ')'" << endl;
			}
		}
		else {
			SIGN = 1;
			cout << "������ if �����ȱ�������� '('" << endl;
		}
	}
	else {
		SIGN = 1;
		cout << "������ if �����ȱ�� 'if' �ؼ���" << endl;
	}
}





void E() {
	if (SIGN == 0) {
		if (s[i] == '(' || s[i] == 'i') {
			T();
			if (SIGN == 0) {
				if (s[i] == '+' || s[i] == '-') {
					E_(); // ��������ļӼ�����
				}
				else if (s[i] == ')' || s[i] == '#') {
					return; // �Ϸ�����
				}
				else {
					SIGN = 1;
					cout << "E����������Ч�ַ�" << endl;
				}
			}
		}
		else {
			SIGN = 1;
			cout << "E���󣺱��ʽӦ��'('�������ʼ" << endl;
		}
	}
}


void E_() {
	if (SIGN != 0) return;

	// ������� '+' �� '-' ��������������һ�����ʽ
	while (s[i] == '+' || s[i] == '-') {
		i++; // ����������
		T(); // ������һ����
		if (SIGN != 0) return; // �������ʧ�ܣ���������
	}

	// �Ϸ�����ֹ��Ӧ���������� ')' ������� '#'
	if (s[i] != ')' && s[i] != '#' && s[i] != '}') {
		SIGN = 1;
		cout << "������ E' ��������Ч�ַ�" << endl;
	}
}


void T() {
	if (SIGN != 0) return;

	// ��������Ƿ��� '(' �� 'i'
	if (s[i] == '(' || s[i] == 'i') {
		F(); // ��������
		if (SIGN != 0) return;

		// ����˳�������
		while (s[i] == '*' || s[i] == '/') {
			T_(); // �����˳�
			if (SIGN != 0) return;
		}
	}
	else {
		SIGN = 1;
		cout << "������ T ��������Ч�ַ���ȱ������" << endl;
	}
}


void T_()
{
	// �ȼ���Ƿ����﷨����
	if (SIGN != 0) return;
	printf("T'   ");

	// �жϵ�ǰ�ַ��Ƿ�Ϊ '*' �� '/'����ʾ�к����ĳ˳�������
	while (s[i] == '*' || s[i] == '/') {
		M(); // �����˳�������
		if (SIGN != 0) return; // �������ʧ���򷵻�

		// ���������������ӣ�F��
		if (s[i] == '(' || s[i] == 'i') {
			F(); // ��������
		}
		else {
			SIGN = 1;
			cout << "������ T' ��ȱ����Ч�����ӣ�F��" << endl;
			return;
		}

		// ����Ƿ�������������� '*' �� '/' �����
		if (SIGN != 0) return;
	}

	// ��� T' �Ľ���������Ӧ��Ϊ '+'��'-'��')' �� '#'
	if (s[i] != '+' && s[i] != '-' && s[i] != ')' && s[i] != '#') {
		SIGN = 1;
		cout << "������ T' ��������Ч�ַ�" << endl;
	}
}


void F()
{
	// �ȼ���Ƿ����﷨����
	if (SIGN != 0) return;
	printf("F   ");

	// �жϵ�ǰ�ַ��Ƿ�Ϊ '('����ʾ������һ�������ڵı��ʽ
	if (s[i] == '(') {
		i++; // ���� '('

		// ���������ڵı��ʽ
		if (s[i] == '(' || s[i] == 'i') {
			E(); // ���� E ���������ڵ�����
		}
		else {
			SIGN = 1;
			cout << "������ F ��ȱ����Ч�ı��ʽ" << endl;
			return;
		}

		// ����Ƿ���ƥ��� ')'
		if (SIGN == 0 && s[i] == ')') {
			i++; // ���� ')'
		}
		else {
			SIGN = 1;
			cout << "������ F ��ȱ�������� ')'" << endl;
			return;
		}
	}
	// �����ǰ�ַ��� 'i'����ʾһ������
	else if (s[i] == 'i') {
		i++; // �������� 'i'
	}
	else {
		// ��ǰ�ַ��Ȳ��� '(' Ҳ���� 'i'����Ǵ���
		SIGN = 1;
		cout << "������ F ��������Ч�ַ�" << endl;
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
			cout << "A����" << endl;
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
			cout << "M����" << endl;
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
			cout << "V����" << endl;
		}
	}
}

int main()
{
	FILE* fp;
	string str, med;
	char arr[MAX];

	//printf("�������Ϊ��\n");
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
			int flag = 0;//����������ż��
			if (arr[1] == '2') //��������ʱ
			{
				med += 'i';

			}

			for (int i = 0; i < len; i++)
			{
				if (arr[i] == ',' && flag == 0) //�ҵ�����
				{
					i++;
					while (arr[i] != '\0') {
						str += arr[i];
						if (arr[1] != '2') {
							med += arr[i];
						}
						if (arr[i + 2] == '\0')//���һ���ַ�
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

	cout << "��������Ϊ��" << str << endl;
	cout << "ת��Ϊ��" << med << endl;

	SIGN = 0;
	i = 0;

	// �� like ת��Ϊ�ַ����� s
	strncpy(s, med.c_str(), sizeof(s) - 1); // ʹ�� strncpy ���а�ȫ����
	s[sizeof(s) - 1] = '\0'; // ȷ���ַ����� '\0' ��β

	// ��������Ƿ�ֻ�н����� '#'
	if (s[0] == '#') {
		return 0;
	}

	// ���ý������� S �����﷨����
	S();

	// �����������ȷ������Ƿ�Ϸ�
	if (s[i] == '#' && SIGN == 0) {
		printf("\n���Ϸ�\n");
	}
	else {
		printf("\n��䲻�Ϸ�\n");
	}

	return 1;
}

