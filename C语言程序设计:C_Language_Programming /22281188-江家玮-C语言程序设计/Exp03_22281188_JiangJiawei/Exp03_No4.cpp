#include <stdio.h>
#include<math.h>

#define Pi 3.1415926

int main() {
    // 1.
    int result1 = 5 * (0x10 + 12 / 3) - 012 + 0x2F;
    printf("Result of expression 1: %d\n", result1);

    //2.
    double result2 = sin(30 * 3.14159 / 180) - (int)cos(60 * 3.14159 / 180);
    printf("Result of expression 2: %.2f\n", result2);
        
    //3.
    double result3 = log(pow('z' - 'a' + 1, 2)) + log10(pow(10, 3));
    printf("Result of expression 3: %.4f\n", result3);
    
    //4.
    double result4 = sqrt(Pi * Pi + 1);
    printf("Result of expression 4: %.4f\n", result4);
    
    //5.
    double result5 = 1.0 * 23.582 / (7.96 / 3.67);
    printf("Result of expression 5: %.4f\n", result5);
}
