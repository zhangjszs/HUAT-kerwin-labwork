#include<stdio.h>
#include<math.h> // 添加了数学函数头文件

int main()
{
    float a, b, x;
    printf("Please input a,b:");
    scanf("%f,%f", &a, &b); // 修改了格式字符串和取地址符
    // x = a % b; // 错误的代码
    x = fmod(a, b); // 正确的代码，使用fmod函数
    printf("x=%f\n", x);
    return 0;
}
