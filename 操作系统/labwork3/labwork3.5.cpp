#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Pipe creation failed.\n");
        return 1;
    }

    p1 = fork();

    if (p1 < 0) {
        fprintf(stderr, "Fork failed.\n");
        return 1;
    } else if (p1 == 0) {
        // 子进程1，向管道写入消息
        close(pipefd[0]); // 关闭读取端

        char* message = "Child 1 is sending message!";
        write(pipefd[1], message, strlen(message) + 1); // 写入消息到管道

        close(pipefd[1]); // 关闭写入端
        exit(0);
    }

    p2 = fork();

    if (p2 < 0) {
        fprintf(stderr, "Fork failed.\n");
        return 1;
    } else if (p2 == 0) {
        // 子进程2，向管道写入消息
        close(pipefd[0]); // 关闭读取端

        char* message = "Child 2 is sending message!";
        write(pipefd[1], message, strlen(message) + 1); // 写入消息到管道

        close(pipefd[1]); // 关闭写入端
        exit(0);
    }

    // 父进程，从管道读取消息
    close(pipefd[1]); // 关闭写入端

    char buffer[256];
    int bytesRead;

    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        printf("Parent: Received message: %s\n", buffer);
    }

    close(pipefd[0]); // 关闭读取端

    wait(NULL); // 等待子进程结束
    wait(NULL);

    return 0;
}