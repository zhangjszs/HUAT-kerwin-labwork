#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid;
    char buf[1024];
    ssize_t bytesRead;

    // 创建两个管道
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 创建子进程
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // 子进程
        // 关闭不需要的管道端
        close(pipe1[1]);
        close(pipe2[0]);

        // 从父进程接收数据
        bytesRead = read(pipe1[0], buf, sizeof(buf) - 1);
        if (bytesRead > 0) {
            buf[bytesRead] = '\0';
            std::cout << "Child received: " << buf << std::endl;

            // 向父进程发送数据
            const char *reply = "Message from child";
            write(pipe2[1], reply, strlen(reply));
        }

        // 关闭管道并退出
        close(pipe1[0]);
        close(pipe2[1]);
        exit(EXIT_SUCCESS);
    } else { // 父进程
        // 关闭不需要的管道端
        close(pipe1[0]);
        close(pipe2[1]);

        // 向子进程发送数据
        const char *message = "Message from parent";
        write(pipe1[1], message, strlen(message));

        // 从子进程接收数据
        bytesRead = read(pipe2[0], buf, sizeof(buf) - 1);
        if (bytesRead > 0) {
            buf[bytesRead] = '\0';
            std::cout << "Parent received: " << buf << std::endl;
        }

        // 等待子进程并清理资源
        waitpid(pid, NULL, 0);

        // 关闭管道
        close(pipe1[1]);
        close(pipe2[0]);
        exit(EXIT_SUCCESS);
    }
}