/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-10 22:18:43
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-10 22:18:53
 * @FilePath: \HUAT-kerwin-labwork\操作系统\labwork\labwork1.5.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
int main()
{
    pid_t child_pid1, child_pid2;

    // 创建第一个子进程，执行指定路径下的可执行文件
    child_pid1 = fork();
    if (child_pid1 < 0)
    {
        perror("fork failed");
        exit(1);
    }
    if (child_pid1 == 0)
    {
        // 子进程1的代码，执行指定路径下的可执行文件
        // 确保 "lab1" 程序具有执行权限
        char *argv[] = {"/home/kerwin-zhang/study/coding/operatingsystem/lab1", NULL};
        execv(argv[0], argv); // 使用 execv 来执行 "lab1" 程序
        perror("execv failed");
        exit(1);
    }
    // 等待第一个子进程结束
    int status;
    waitpid(child_pid1, &status, 0); // 阻塞等待
    // 创建第二个子进程，它将暂停10秒后退出
    child_pid2 = fork();
    if (child_pid2 < 0)
    {
        perror("fork failed");
        exit(1);
    }
    if (child_pid2 == 0)
    {
        // 子进程2的代码，暂停10秒后退出
        sleep(10);
        exit(0);
    }
    // 父进程非阻塞等待第二个子进程退出
    while (waitpid(child_pid2, &status, WNOHANG) == 0)
    {
        // 检查子进程是否已经退出
        if (WIFEXITED(status))
        {
            // 子进程已退出，收集其退出状态并退出循环
            printf("Child process 2 exited with status %d\n", WEXITSTATUS(status));
            break;
        }
        // 否则，继续等待
    }
    // 父进程结束
    return 0;
}
