/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-10 12:10:49
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-10 12:10:53
 * @FilePath: \HUAT-kerwin-labwork\操作系统\labwork\labwork1.3.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define NUM_CHILDREN 2 // 定义每个进程可以创建的子进程数量
void create_process_tree(int depth, int parent_pid)
{
    if (depth <= 0)
    {
        return; // 达到递归深度，停止创建更多子进程
    }
    pid_t child_pid = fork();
    if (child_pid < 0)
    {
        perror("fork failed");
        exit(1);
    }
    else if (child_pid == 0)
    {
        // 子进程的代码
        printf("Child process (PID: %d, PPID: %d) created at depth %d\n", getpid(), getppid(), depth);
        // 递归创建子进程
        create_process_tree(depth - 1, getpid());
        exit(0); // 子进程结束
    }
    // 父进程的代码
    printf("Parent process (PID: %d) creating child at depth %d\n", parent_pid, depth);
    wait(NULL); // 等待子进程结束
    // 递归创建下一个子进程
    create_process_tree(depth - 1, child_pid);
}
int main()
{
    pid_t root_pid = fork();
    if (root_pid < 0)
    {
        perror("fork failed");
        exit(1);
    }
    else if (root_pid == 0)
    {
        printf("Root process (PID: %d)\n", getpid());
        create_process_tree(3, getpid()); // 创建深度为 3 的进程树
        exit(0);                          // 根进程结束
    }
    // 等待根进程结束
    wait(NULL);
    return 0;
}
