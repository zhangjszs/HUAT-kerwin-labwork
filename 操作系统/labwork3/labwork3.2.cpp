#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int k = 1;

void func(int signo) {
    k = 0;
}

int main() {
    signal(SIGINT, func);
    while (k != 0) {
        // do nothing
    }
    printf("\n main process is killed by keybreak!\n");
    return 0;
}