#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int k;
void func(int signo)
{
	k = 0;
}
int main()
{
	int p;
	while ((p = fork()) == -1);
	if (p == 0)
	{
		k = 1;
		signal(10, func);
		while (k != 0)
			;
		printf("Child process is killed by parent!\n");
	}
	else
	{
		sleep(1);
		kill(p, 10);
		wait(0);
		printf("Parent process has killed!\n");
	}
	return 0;
}
