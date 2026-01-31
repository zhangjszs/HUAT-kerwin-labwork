#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>

int ticketAvailable = 0;  // 可用票数

void driverHandler(int signum) {
    printf("Driver: Got signal from ticket seller.\n");
    ticketAvailable++;
}

void sellerHandler(int signum) {
    printf("Ticket Seller: Got signal from driver.\n");
    if (ticketAvailable > 0) {
        printf("Ticket Seller: Selling a ticket.\n");
        ticketAvailable--;
    } else {
        printf("Ticket Seller: No ticket available.\n");
    }
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed.\n");
        return 1;
    } else if (pid == 0) {
        // 子进程，售票员
        signal(SIGUSR1, sellerHandler);

        while (1) {
            printf("Ticket Seller: Waiting for driver's signal.\n");
            sleep(1);  // 等待司机发出信号
            kill(getppid(), SIGUSR1);  // 向父进程发送信号
            usleep(500000);  // 延时0.5秒
        }
    } else {
        // 父进程，司机
        signal(SIGUSR1, driverHandler);

        while (1) {
            printf("Driver: Waiting for ticket seller's signal.\n");
            sleep(1);  // 等待售票员发出信号
            kill(pid, SIGUSR1);  // 向子进程发送信号
            usleep(500000);  // 延时0.5秒
        }
    }

    return 0;
}