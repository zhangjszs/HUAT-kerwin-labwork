#include "add.h"
#include "sub.h"
#include <stdio.h>
int main()
{
    int x, y;
    float a, b;
    x = 5;
    y = 2;
    a = 5.5;
    b = 2.2;
    printf("%d + %d = %d\n", x, y, add_int(x, y));
    printf("%3.1f + %3.1f = %3.1f\n", a, b, add_float(a, b));
    printf("%d - %d = %d\n", x, y, sub_int(x, y));
    printf("%3.1f - %3.1f = %3.1f\n", a, b, sub_float(a, b));
    return 0;
}
