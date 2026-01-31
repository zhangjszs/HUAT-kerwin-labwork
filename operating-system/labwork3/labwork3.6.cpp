#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/wait.h>

int main()
{
    int fd[2];
    pid_t pid1, pid2;
    char message1[] = "Message for Child 1";
    char message2[] = "Message for Child 2";

    // 创建管道
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 创建第一个子进程
    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0)
    {                 // 子进程1
        close(fd[1]); // 关闭写端

        // 读取父进程发送的信息
        char buffer[100];
        ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer) - 1);
        if (bytesRead == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        buffer[bytesRead] = '\0'; // 确保字符串正确终止
        std::cout << "Child 1 received: " << buffer << std::endl;

        close(fd[0]); // 关闭读端
        exit(EXIT_SUCCESS);
    }
    else
    {
        // 创建第二个子进程
        pid2 = fork();
        if (pid2 == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0)
        {                 // 子进程2
            close(fd[1]); // 关闭写端

            // 读取父进程发送的信息
            char buffer[100];
            ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer) - 1);
            if (bytesRead == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
            buffer[bytesRead] = '\0'; // 确保字符串正确终止
            std::cout << "Child 2 received: " << buffer << std::endl;

            close(fd[0]); // 关闭读端
            exit(EXIT_SUCCESS);
        }
        else
        {                 // 父进程
            close(fd[0]); // 关闭读端

            // 向子进程发送信息
            if (write(fd[1], message1, strlen(message1)) == -1)
            {
                perror("write to child 1");
            }

            // 等待子进程1接收完毕
            waitpid(pid1, NULL, 0);

            // 向子进程发送信息
            if (write(fd[1], message2, strlen(message2)) == -1)
            {
                perror("write to child 2");
            }

            // 等待子进程2接收完毕
            waitpid(pid2, NULL, 0);

            close(fd[1]); // 关闭写端
            exit(EXIT_SUCCESS);
        }
    }
}