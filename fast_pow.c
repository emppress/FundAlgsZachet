#include "stdio.h"

// O(logn)
int fast_pow(int a, int degree)
{
    int res = 1;
    while (degree)
    {
        if (degree & 1)
        {
            res *= a;
            --degree;
        }
        else
        {
            a *= a;
            degree >>= 1; // 10 ^ 4 = 10 ^ (100)2
        }
    }
    return res;
}

int main()
{
    int res = fast_pow(10, 9);
    printf("%d", res);

    return 0;
}