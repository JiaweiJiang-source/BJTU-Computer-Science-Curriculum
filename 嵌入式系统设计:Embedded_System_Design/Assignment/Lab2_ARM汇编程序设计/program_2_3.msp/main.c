#define UINT unsigned int
extern UINT gcd_asm(UINT a, UINT b);

int main(void)
{

    gcd_asm(15, 9);
    return 0;
}