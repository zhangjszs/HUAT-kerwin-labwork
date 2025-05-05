#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> // 添加 read/write/close 函数声明

#define DEVICE_NAME "/dev/Dev_frame" // 根据内核模块的 DEVICE_NAME 填写设备节点路径
#define M_LENGTH 10

int main()
{
    int fd, i, j;
    char c = 'a';
    char app_data0[M_LENGTH][M_LENGTH];
    char app_data1[M_LENGTH][M_LENGTH];

    // 以读写模式打开设备文件（不需要 O_CREAT 因为设备节点应由 mknod 创建）
    fd = open(DEVICE_NAME, O_RDWR);
    if (fd == -1)
    {
        printf("%s device open failure.\n", DEVICE_NAME);
        return -1;
    }

    printf("Reading data.\n");
    // 从设备文件读取数据到 app_data0
    read(fd, app_data0, sizeof(app_data0));

    // 打印从内核读取的数据
    for (i = 0; i < M_LENGTH; i++)
    {
        for (j = 0; j < M_LENGTH; j++)
        {
            app_data1[i][j] = c + i;        // 给用户空间数据重新赋值
            printf("%c ", app_data0[i][j]); // 打印内核传来的数据
        }
        printf("\n");
    }

    printf("Writing data.\n");
    // 将用户空间数据写入设备文件
    write(fd, app_data1, sizeof(app_data1));

    printf("Device closed.\n");
    close(fd); // 关闭设备文件
    return 0;
}
