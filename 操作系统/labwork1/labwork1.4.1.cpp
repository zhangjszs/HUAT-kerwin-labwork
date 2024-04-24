/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-10 22:17:33
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-10 22:17:55
 * @FilePath: \HUAT-kerwin-labwork\操作系统\labwork\labwork1.4.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child;
    int p;

    while ((child = fork()) == -1);

    if (child == 0) {
        printf("In child: sleep for 10 seconds and then exit.\n");
        sleep(10);
        exit(0);
    } else {
        do {
            p = waitpid(child, NULL, WNOHANG);
            if (p == 0) {
                printf("In father: The child process has not exited.\n");
                sleep(1);
            }
        } while (p == 0);

        if (p == child) {
            printf("Get child exit code then exit!\n");
        } else {
            printf("Error occurred!\n");
        }
    }

    exit(0);
}
