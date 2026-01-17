#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "Dev_frame"
int DEVICE_MAJOR = 0;

#define M_LENGTH 10
char core_data0[M_LENGTH][M_LENGTH];
char core_data1[M_LENGTH][M_LENGTH];

int device_open(struct inode *inode, struct file *filp)
{
    char c = 'a';
    int i, j;
    printk("user open device.\n");
    for (i = 0; i < M_LENGTH; i++)
    {
        for (j = 0; j < M_LENGTH; j++)
        {
            core_data0[i][j] = c + j;
        }
    }
    return 0;
}

ssize_t device_write(struct file *filp, const char *buffer, size_t count, loff_t *f_pos)
{
    int i, j;
    printk("user write data to driver.\n");
    copy_from_user(core_data1, buffer, sizeof(core_data1));
    for (i = 0; i < M_LENGTH; i++)
    {
        for (j = 0; j < M_LENGTH; j++)
        {
            printk("%c", core_data1[i][j]);
        }
        printk("\n");
    }
    return count;
}

ssize_t device_read(struct file *filp, char *buffer, size_t count, loff_t *f_pos)
{
    printk("user read data from driver.\n");
    copy_to_user(buffer, core_data0, sizeof(core_data0));
    return count;
}

int device_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    printk("user ioctl running.\n");
    switch (cmd)
    {
    case 1:
        printk("cmd = %d.\n", cmd);
        break;
    case 2:
        printk("cmd = %d.\n", cmd);
        break;
    case 3:
        printk("cmd = %d.\n", cmd);
        break;
    case 4:
        printk("cmd = %d.\n", cmd);
        break;
    case 5:
        printk("cmd = %d.\n", cmd);
        break;
    }
    return cmd;
}

int device_release(struct inode *inode, struct file *filp)
{
    printk("device release\n");
    return 0;
}

struct file_operations device_fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .open = device_open,
    .write = device_write,
    .release = device_release,
    // .ioctl = device_ioctl, // 根据需求决定是否取消注释
};

int device_init(void)
{
    int ret;
    ret = register_chrdev(0, DEVICE_NAME, &device_fops);
    if (ret < 0)
    {
        printk("register chrdev failure!\n");
        return ret;
    }
    else
    {
        printk("register chrdev ok!\n");
        DEVICE_MAJOR = ret;
    }
    return 0;
}

void device_exit(void)
{
    unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
    printk("unregister chrdev ok!\n");
}

module_init(device_init);
module_exit(device_exit);

MODULE_LICENSE("GPL");
