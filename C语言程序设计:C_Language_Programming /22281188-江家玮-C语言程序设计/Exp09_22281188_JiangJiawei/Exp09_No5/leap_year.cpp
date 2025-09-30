#include "leap_year.h"

// 判断是否为闰年
int isLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        return 1; // 是闰年
    }
    else {
        return 0; // 不是闰年
    }
}