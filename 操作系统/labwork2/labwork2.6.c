#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h> 

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0; // 缓冲区中的数据数量

sem_t empty_sem; // 空缓冲区的信号量
sem_t full_sem;  // 满缓冲区的信号量
pthread_mutex_t mutex; // 互斥锁

void *producer(void *arg)
{
    int item = 1;
    while (1)
    {
        // 等待空缓冲区
        sem_wait(&empty_sem);
        
        // 获取互斥锁
        pthread_mutex_lock(&mutex);

        // 生产数据
        buffer[count] = item;
        printf("Produced: %d\n", item);
        count++;

        // 释放互斥锁
        pthread_mutex_unlock(&mutex);

        // 发送满缓冲区信号
        sem_post(&full_sem);

        item++;

        // 模拟生产时间
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg)
{
    while (1)
    {
        // 等待满缓冲区
        sem_wait(&full_sem);

        // 获取互斥锁
        pthread_mutex_lock(&mutex);

        // 消费数据
        int item = buffer[count - 1];
        printf("Consumed: %d\n", item);
        count--;

        // 释放互斥锁
        pthread_mutex_unlock(&mutex);

        // 发送空缓冲区信号
        sem_post(&empty_sem);

        // 模拟消费时间
        sleep(2);
    }
    return NULL;
}

int main()
{
    pthread_t producer_thread, consumer_thread;

    // 初始化信号量和互斥锁
    sem_init(&empty_sem, 0, BUFFER_SIZE);
    sem_init(&full_sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // 创建生产者线程和消费者线程
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // 等待线程结束
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // 销毁信号量和互斥锁
    sem_destroy(&empty_sem);
    sem_destroy(&full_sem);
    pthread_mutex_destroy(&mutex);

    return 0;
}
