#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_process_1(int sig) {
    std::cout << "Child process 1 is killed by parent!" << std::endl;
    exit(0);
}

void child_process_2(int sig) {
    std::cout << "Child process 2 is killed by parent!" << std::endl;
    exit(0);
}

void parent_process(int sig) {
    int status;
    kill(0, SIGINT); // 向所有子进程发送信号
    waitpid(-1, &status, 0); // 等待第一个子进程终止
    waitpid(-1, &status, 0); // 等待第二个子进程终止
    std::cout << "Parent process is killed!" << std::endl;
    exit(0);
}

int main() {
    pid_t pid1, pid2;

    // 创建子进程1
    if ((pid1 = fork()) == 0) {
        signal(SIGINT, child_process_1);
        while(1); // 子进程1进入死循环
    }

    // 创建子进程2
    if ((pid2 = fork()) == 0) {
        signal(SIGINT, child_process_2);
        while(1); // 子进程2进入死循环
    }

    // 父进程捕捉SIGINT信号
    signal(SIGINT, parent_process);

    // 父进程等待键盘中断信号
    while(1);

    return 0;
}