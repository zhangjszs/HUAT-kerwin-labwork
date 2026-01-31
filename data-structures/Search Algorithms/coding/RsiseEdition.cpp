#include <stdio.h>
#define Max 6
#define HashMax 5

int Data[Max] = {12, 160, 219, 522, 725, 9997}; // 数据数组
int HashTab[HashMax];
int Counter = 1; // 计数器

int Hash_Mod(int Key) // 哈希函数之除留余数法
{
    return Key % HashMax; // 返回，键值除以哈希表大小取余数
}

int Collision_Offset(int Address) // 差值哈希冲突解决法
{
    int offset = 3;                      // 设差值为3
    return (Address + offset) % HashMax; // 返回，旧地址加差值除以哈希表大小取余数
}

int Create_Hash(int Key) // 建立哈希表
{
    int Address = Hash_Mod(Key); // 计算数据地址

    if (HashTab[Address] == 0) // 如果地址为空，直接存储数据
    {
        HashTab[Address] = Key;
        return 1;
    }
    else // 发生冲突，使用差值解决法找到下一个可用地址
    {
        int offset = Collision_Offset(Address);
        while (HashTab[offset] != 0)
        {
            offset = Collision_Offset(offset);
        }
        HashTab[offset] = Key;
        return 1;
    }

    return 0;
}

int Hash_Search(int Key) // 哈希查找法
{
    int Address; // 数据地址

    Counter = 0;
    Address = Hash_Mod(Key); // 调用哈希函数
    while (Counter < HashMax)
    {
        Counter++;
        if (HashTab[Address] == Key)
            return 1;
        else
            Address = Collision_Offset(Address); // 调用冲突解决法
    }
    return 0;
}

int main()
{
    int KeyValue; // 欲查找数据变量
    int Index;    // 输入数据索引
    int i;

    Index = 0;

    printf("原始的数据为："); // 输出输入数据
    for (i = 0; i < Max; i++)
        printf("[%d]", Data[i]);
    printf("\n");

    for (i = 0; i < HashMax; i++) // 哈希表初始化
        HashTab[i] = 0;

    while (Index < Max)
    {
        if (Create_Hash(Data[Index]))
            printf("哈希存储成功!!\n"); // 哈希建立成功
        else
            printf("哈希存储失败!!\n"); // 哈希建立失败
        Index++;
    }
    for (i = 0; i < HashTab[i]; i++) // 输出哈希数据
    {
        printf("[%d]", HashTab[i]);
    }
    printf("\n");

    while (KeyValue != -1) // 输入-1结束程序
    {
        printf("请输入要查找的值：");
        scanf("%d", &KeyValue);

        if (Hash_Search(KeyValue))
            printf("查找次数 = %d 次，查找成功！\n", Counter); // 输出查找次数
        else
            printf("查找失败，没有这个数据！\n"); // 输出没有找到数据
    }
    return 0;
}
