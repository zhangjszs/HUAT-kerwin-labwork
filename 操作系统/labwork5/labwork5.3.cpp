#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0
#define INVALID -1
#define NULL 0

#define total_instruction 320 /*指令流长*/
#define total_vp 32           /*虚页长*/
#define clear_period 50       /*清0周期*/

typedef struct /*页面结构*/
{
    int pn, pfn, counter, time;
} pl_type;
pl_type pl[total_vp]; /*页面结构数组*/

struct pfc_struct
{ /*页面控制结构*/
    int pn, pfn;
    struct pfc_struct *next;
};

typedef struct pfc_struct pfc_type;

pfc_type pfc[total_vp], *freepf_head, *busypf_head, *busypf_tail;

int diseffect, a[total_instruction];
int page[total_instruction], offset[total_instruction];

int initialize(int);
int FIFO(int);
int LRU(int);
int LFU(int);
int NUR(int);
int OPT(int);

int main()
{
    int s, i, j;
    srand(10 * getpid());                            /*由于每次运行时进程号不同，故可用来作为初始化随机数队列的“种子”*/
    s = (float)319 * rand() / 32767 / 32767 / 2 + 1; //
    for (i = 0; i < total_instruction; i += 4)       /*产生指令队列*/
    {
        if (s < 0 || s > 319)
        {
            printf("When i==%d,Error,s==%d\n", i, s);
            exit(0);
        }
        a[i] = s;                                            /*任选一指令访问点m*/
        a[i + 1] = a[i] + 1;                                 /*顺序执行一条指令*/
        a[i + 2] = (float)a[i] * rand() / 32767 / 32767 / 2; /*执行前地址指令m' */
        a[i + 3] = a[i + 2] + 1;                             /*顺序执行一条指令*/

        s = (float)(318 - a[i + 2]) * rand() / 32767 / 32767 / 2 + a[i + 2] + 2;
        if ((a[i + 2] > 318) || (s > 319))
            printf("a[%d+2],a number which is :%d and s==%d\n", i, a[i + 2], s);
    }
    for (i = 0; i < total_instruction; i++) /*将指令序列变换成页地址流*/
    {
        page[i] = a[i] / 10;
        offset[i] = a[i] % 10;
    }
    for (i = 4; i <= 32; i++) /*用户内存工作区从4个页面到32个页面*/
    {
        printf("---%2d page frames---\n", i);
        FIFO(i);
        LRU(i);
        LFU(i);
        NUR(i);
        OPT(i);
    }
    return 0;
}
int initialize(int total_pf) 
{
    int i;
    diseffect = 0;
    for (i = 0; i < total_vp; i++)
    {
        pl[i].pn = i;
        pl[i].pfn = INVALID; /*置页面控制结构中的页号，页面为空*/
        pl[i].counter = 0;
        pl[i].time = -1; /*页面控制结构中的访问次数为0，时间为-1*/
    }
    for (i = 0; i < total_pf - 1; i++)
    {
        pfc[i].next = &pfc[i + 1];
        pfc[i].pfn = i;
    } /*建立pfc[i-1]和pfc[i]之间的链接*/
    pfc[total_pf - 1].next = NULL;
    pfc[total_pf - 1].pfn = total_pf - 1;
    freepf_head = &pfc[0]; /*空页面队列的头指针为pfc[0]*/

    return 0;
}

int FIFO(int total_pf) /*先进先出算法*/ /*用户进程的内存页面数*/
{
    int i, j;
    pfc_type *p;
    initialize(total_pf);             /*初始化相关页面控制用数据结构*/
    busypf_head = busypf_tail = NULL; /*忙页面队列头，队列尾链接*/
    for (i = 0; i < total_instruction; i++)
    {
        if (pl[page[i]].pfn == INVALID) /*页面失效*/
        {
            diseffect += 1;          /*失效次数*/
            if (freepf_head == NULL) /*无空闲页面*/
            {
                p = busypf_head->next;
                pl[busypf_head->pn].pfn = INVALID;
                freepf_head = busypf_head; /*释放忙页面队列的第一个页面*/
                freepf_head->next = NULL;
                busypf_head = p;
            }
            p = freepf_head->next; /*按FIFO方式调新页面入内存页面*/
            freepf_head->next = NULL;
            freepf_head->pn = page[i];
            pl[page[i]].pfn = freepf_head->pfn;

            if (busypf_tail == NULL)
                busypf_head = busypf_tail = freepf_head;
            else
            {
                busypf_tail->next = freepf_head; /*free页面减少一个*/
                busypf_tail = freepf_head;
            }
            freepf_head = p;
        }
    }
    printf("FIFO:%6.4f\n", 1 - (float)diseffect / 320);

    return 0;
}

int LRU(int total_pf) /*最近最久未使用算法*/
{
    int min, minj, i, j, present_time;
    initialize(total_pf);
    present_time = 0;

    for (i = 0; i < total_instruction; i++)
    {
        if (pl[page[i]].pfn == INVALID) /*页面失效*/
        {
            diseffect++;
            if (freepf_head == NULL) /*无空闲页面*/
            {
                min = 32767;
                for (j = 0; j < total_vp; j++) /*找出time的最小值*/
                    if (min > pl[j].time && pl[j].pfn != INVALID)
                    {
                        min = pl[j].time;
                        minj = j;
                    }
                freepf_head = &pfc[pl[minj].pfn]; // 腾出一个单元
                pl[minj].pfn = INVALID;
                pl[minj].time = -1;
                freepf_head->next = NULL;
            }
            pl[page[i]].pfn = freepf_head->pfn; // 有空闲页面,改为有效
            pl[page[i]].time = present_time;
            freepf_head = freepf_head->next; // 减少一个free 页面
        }
        else
            pl[page[i]].time = present_time; // 命中则增加该单元的访问次数

        present_time++;
    }
    printf("LRU:%6.4f\n", 1 - (float)diseffect / 320);
    return 0;
}

int NUR(int total_pf) /*最近未使用算法*/
{
    int i, j, dp, cont_flag, old_dp;
    pfc_type *t;
    initialize(total_pf);
    dp = 0;
    for (i = 0; i < total_instruction; i++)
    {
        if (pl[page[i]].pfn == INVALID) /*页面失效*/
        {
            diseffect++;
            if (freepf_head == NULL) /*无空闲页面*/
            {
                cont_flag = TRUE;
                old_dp = dp;
                while (cont_flag)
                    if (pl[dp].counter == 0 && pl[dp].pfn != INVALID)
                        cont_flag = FALSE;
                    else
                    {
                        dp++;
                        if (dp == total_vp)
                            dp = 0;
                        if (dp == old_dp)
                            for (j = 0; j < total_vp; j++)
                                pl[j].counter = 0;
                    }
                freepf_head = &pfc[pl[dp].pfn];
                pl[dp].pfn = INVALID;
                freepf_head->next = NULL;
            }
            pl[page[i]].pfn = freepf_head->pfn;
            freepf_head = freepf_head->next;
        }
        else
            pl[page[i]].counter = 1;
        if (i % clear_period == 0)
            for (j = 0; j < total_vp; j++)
                pl[j].counter = 0;
    }
    printf("NUR:%6.4f\n", 1 - (float)diseffect / 320);

    return 0;
}

int OPT(int total_pf) /*最佳置换算法*/
{
    int i, j, max, maxpage, d, dist[total_vp];
    pfc_type *t;
    initialize(total_pf);
    for (i = 0; i < total_instruction; i++)
    {
        if (pl[page[i]].pfn == INVALID) /*页面失效*/
        {
            diseffect++;
            if (freepf_head == NULL) /*无空闲页面*/
            {
                for (j = 0; j < total_vp; j++)
                    if (pl[j].pfn != INVALID)
                        dist[j] = 32767; /* 最大"距离" */
                    else
                        dist[j] = 0;
                d = 1;
                for (j = i + 1; j < total_instruction; j++)
                {
                    if (pl[page[j]].pfn != INVALID)
                        dist[page[j]] = d;
                    d++;
                }
                max = -1;
                for (j = 0; j < total_vp; j++)
                    if (max < dist[j])
                    {
                        max = dist[j];
                        maxpage = j;
                    }
                freepf_head = &pfc[pl[maxpage].pfn];
                freepf_head->next = NULL;
                pl[maxpage].pfn = INVALID;
            }
            pl[page[i]].pfn = freepf_head->pfn;
            freepf_head = freepf_head->next;
        }
    }
    printf("OPT:%6.4f\n", 1 - (float)diseffect / 320);

    return 0;
}

int LFU(int total_pf) /*最不经常使用置换法*/
{
    int i, j, min, minpage;
    pfc_type *t;
    initialize(total_pf);
    for (i = 0; i < total_instruction; i++)
    {
        if (pl[page[i]].pfn == INVALID) /*页面失效*/
        {
            diseffect++;
            if (freepf_head == NULL) /*无空闲页面*/
            {
                min = 32767;
                for (j = 0; j < total_vp; j++)
                {
                    if (min > pl[j].counter && pl[j].pfn != INVALID)
                    {
                        min = pl[j].counter;
                        minpage = j;
                    }
                    pl[j].counter = 0;
                }
                freepf_head = &pfc[pl[minpage].pfn];
                pl[minpage].pfn = INVALID;
                freepf_head->next = NULL;
            }
            pl[page[i]].pfn = freepf_head->pfn; // 有空闲页面,改为有效
            pl[page[i]].counter++;
            freepf_head = freepf_head->next; // 减少一个free 页面
        }
        else
            pl[page[i]].counter++;
    }
    printf("LFU:%6.4f\n", 1 - (float)diseffect / 320);

    return 0;
}
