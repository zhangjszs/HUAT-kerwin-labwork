/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-16 12:23:23
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-16 12:23:28
 * @FilePath: \HUAT-kerwin-labwork\操作系统\labwork2\labwork2.5.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

sem_t driver_sem, conductor_sem;

void *driver(void *arg)
{
    while (1)
    {
        sem_wait(&driver_sem); // 等待司机信号量
        printf("Driver: Start driving.\n");
        sleep(2); // 模拟驾驶过程
        printf("Driver: Reached the destination.\n");
        sem_post(&conductor_sem); // 唤醒售票员
    }
    return NULL;
}

void *conductor(void *arg)
{
    while (1)
    {
        sem_wait(&conductor_sem); // 等待售票员信号量
        printf("Conductor: Start selling tickets.\n");
        sleep(1); // 模拟售票过程
        printf("Conductor: Tickets sold.\n");
        sem_post(&driver_sem); // 唤醒司机
    }
    return NULL;
}

int main()
{
    pthread_t driver_thread, conductor_thread;

    sem_init(&driver_sem, 0, 1); // 初始化司机信号量，初始值为1
    sem_init(&conductor_sem, 0, 0); // 初始化售票员信号量，初始值为0

    pthread_create(&driver_thread, NULL, driver, NULL); // 创建司机线程
    pthread_create(&conductor_thread, NULL, conductor, NULL); // 创建售票员线程

    pthread_join(driver_thread, NULL); // 等待司机线程结束
    pthread_join(conductor_thread, NULL); // 等待售票员线程结束

    sem_destroy(&driver_sem); // 销毁司机信号量
    sem_destroy(&conductor_sem); // 销毁售票员信号量

    return 0;
}
