#include <ctype.h>
#include "string_operations.h"

// ���ַ���ת��Ϊ��д
void toUpperCase(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}