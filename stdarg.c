#include "stdio.h"
#include <stdlib.h>

#define va_listt char *
#define va_start(ap, parm) (ap) = (char *)(&(parm) + 1)
#define va_arg(ap, type) (((type *)((ap) += sizeof(type) + 1))[-1])
#define va_end(ap) (ap) = NULL;

int sum(int count, ...)
{
    va_listt args;
    va_start(args, count);
    int sum = 0;

    for (int i = 0; i < count; ++i)
    {
        sum += va_arg(args, int);
    }
    return sum;
}

int main()
{
    int a = sum(4, 2, 3, 2, 2);
    return 0;
}