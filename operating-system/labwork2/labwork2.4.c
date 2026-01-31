/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-16 12:23:09
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-16 12:23:13
 * @FilePath: \HUAT-kerwin-labwork\操作系统\labwork2\labwork2.4.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

int ticket_sum = 20;
sem_t mutex; // 声明信号量变量

void *sell_ticket(void *arg)
{
    int i;
    for (i = 0; i < 20; i++)
    {
        sem_wait(&mutex); // 等待信号量，进入临界区
        if (ticket_sum > 0)
        {
            sleep(1);
            printf("sell the %dth\n", 20 - ticket_sum + 1);
            ticket_sum--;
        }
        sem_post(&mutex); // 释放信号量，离开临界区
    }
    return NULL;
}

int main()
{
    int flag, i;
    pthread_t tids[4];

    sem_init(&mutex, 0, 1); // 初始化信号量，初始值为1

    for (i = 0; i < 4; i++)
    {
        flag = pthread_create(&tids[i], NULL, &sell_ticket, NULL); // 创建线程
        if (flag)
        {
            printf("pthread create error, flag=%d", flag);
            return flag;
        }
    }

    sleep(20);

    void *ans;
    for (i = 0; i < 4; i++)
    {
        flag = pthread_join(tids[i], &ans); // 等待线程结束
        if (flag)
        {
            printf("tid=%lu, join error, flag=%d", tids[i], flag);
            return flag;
        }
        printf("ans=0x%llx\n", (unsigned long long)ans);
    }

    sem_destroy(&mutex); // 销毁信号量

    return 0;
}
