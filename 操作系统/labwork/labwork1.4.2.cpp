/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-10 22:18:11
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-10 22:18:33
 * @FilePath: \HUAT-kerwin-labwork\操作系统\labwork\labwork1.4.2.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
    int child, p;
    while ((child = fork()) == -1);
    if (child == 0) {
        execl("/home/kerwinzhang/opsys/labwork/a.out", "", NULL);
        exit(0);
    } else {
        p = waitpid(child, NULL, 0);
        if (p == child) {
            printf("Get child exit code then exit!\n");
        } else {
            printf("Error occurred!\n");
        }
    }
    exit(0);
}
/*
子进程要加载的程序welcome.c：
#include<stdio.h>
Int main()
{	printf("Hello! This is another process.\n");}


*/