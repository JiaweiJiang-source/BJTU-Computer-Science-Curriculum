#include <ctype.h>
#include "string_operations.h"

// 将字符串转换为大写
void toUpperCase(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}