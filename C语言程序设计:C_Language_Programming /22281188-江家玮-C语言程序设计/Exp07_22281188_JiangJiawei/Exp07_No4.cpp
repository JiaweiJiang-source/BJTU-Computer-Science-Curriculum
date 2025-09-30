#include <stdio.h>

double SegFun(double x)
{
    if (x < 1)
        return x;
    else if (x >= 1 && x < 10)
        return 2 * x - 1;
    else if (x >= 10)
        return -3 * x - 11;
}

int main() {
    double x;
    scanf_s("%lf", &x);

    double y = SegFun(x);
    printf("%lf\n", y);
}
