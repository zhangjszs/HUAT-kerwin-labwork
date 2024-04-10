/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-10 12:10:31
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-10 12:10:35
 * @FilePath: \HUAT-kerwin-labwork\操作系统\labwork\labwork1.2.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
    pid_t pid1, pid2;
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid1 == 0) {
        printf("Child process 1 (PID: %d, PPID: %d)\n", getpid(), getppid());
        printf("b\n");
        exit(0);
    }
    wait(NULL);
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid2 == 0) {
        printf("Child process 2 (PID: %d, PPID: %d)\n", getpid(), getppid());
        printf("c\n");
        exit(0);
    }
    printf("Parent process (PID: %d, PPID: %d)\n", getpid(), getppid());
    printf("a\n");
    wait(NULL);
    return 0;
}
