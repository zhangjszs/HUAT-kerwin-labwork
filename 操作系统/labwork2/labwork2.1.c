/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-16 12:20:23
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-16 12:20:56
 * @FilePath: \HUAT-kerwin-labwork\操作系统\labwork2\labwork2.1.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*(1)*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
// 打印机
void printer(char *str){
		while(*str!='\0')
		{	putchar(*str);	
			fflush(stdout);
			str++;
			sleep(1);
		}
		printf("\n"); 
}
// 线程一
void *thread_fun_1(void *arg)
{
		char *str = "hello";
		printer(str); //打印
}
// 线程二
void *thread_fun_2(void *arg)
{
		char *str = "world";
		printer(str); //打印
}
int main(void)
{
		pthread_t tid1, tid2;
		// 创建 2 个线程
		pthread_create(&tid1, NULL, thread_fun_1, NULL);
		pthread_create(&tid2, NULL, thread_fun_2, NULL);
		// 等待线程结束，回收其资源
		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL); 
		return 0;
}
/* (2)
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
sem_t semA;	//声明一个名为semA的信号量变量
// 打印机
void printer(char *str)
{
		sem_wait(&semA);//申请信号量（P操作）
		while(*str!='\0')
		{
			putchar(*str);	
			fflush(stdout);
			str++;
			sleep(1);
		}
		printf("\n"); 
		sem_post(&semA);//释放信号量（V操作）         
}
// 线程一
void *thread_fun_1(void *arg)
{
		char *str = "hello";
		printer(str); //打印
}
// 线程二
void *thread_fun_2(void *arg)
{
		char *str = "world";
		printer(str); //打印
}
int main(void)
{
		pthread_t tid1, tid2;
		if(sem_init(&semA, 0, 1))	//初始化信号量的值为1（二元信号量）
			printf("error sem_init!\n");
		// 创建 2 个线程
		pthread_create(&tid1, NULL, thread_fun_1, NULL);
		pthread_create(&tid2, NULL, thread_fun_2, NULL);
		// 等待线程结束，回收其资源
		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL); 

		return 0;
}
*/