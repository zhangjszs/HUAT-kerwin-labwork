/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-10 12:10:11
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-10 22:16:43
 * @FilePath: \HUAT-kerwin-labwork\操作系统\labwork\labwork1.1.cpp
 * @Description: 进程创建实验 - 演示fork()系统调用和进程同步
 *
 * 功能：创建子进程并使用wait()同步，输出顺序a b c
 * 算法：父进程先创建子进程1，等待其结束，再创建子进程2，等待结束，最后输出a
 */

/**
 * @brief 主函数：演示进程创建和同步
 *
 * 创建两个子进程，使用wait()确保顺序执行
 * 输出结果：b c a
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
    pid_t pid1, pid2;

    // 创建第一个子进程
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed for pid1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // 子进程1
        printf("b\n");
        exit(EXIT_SUCCESS);
    }

    // 父进程等待子进程1结束
    if (wait(NULL) == -1) {
        perror("wait for pid1 failed");
        exit(EXIT_FAILURE);
    }

    // 创建第二个子进程
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed for pid2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // 子进程2
        printf("c\n");
        exit(EXIT_SUCCESS);
    }

    // 父进程输出a
    printf("a\n");

    // 等待子进程2结束
    if (wait(NULL) == -1) {
        perror("wait for pid2 failed");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
