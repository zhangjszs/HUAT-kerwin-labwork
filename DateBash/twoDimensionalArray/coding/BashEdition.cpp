/*
 * @Description: 
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-11-17 16:32:29
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-11-17 16:46:35
 */
#include<stdio.h>
int main()
{
    int Data[100];                      /*存储40位数的整数数组*/
    int Digit;                          /*数据位数变量        */
    int i,j,k;                        /*循环计数变量        */
    int N;                              /*用户输入值          */
	
    for(i=0;i<100;i++)                /*将数组初始值设为0    */
        Data[i]=0;
	
    Data[0]=1;                        /*设第0位数数组为1    */
    Digit=1;                          /*设数据位数为1       */
	
    printf("请输入一个整数：");
    scanf("%d",&N);                 /*读取用户欲求的N值   */
	
    for(i=1;i<N+1;i++)
    {
        for(j=0;j<Digit;j++)
            Data[j]*=i;               /*数组中内容的运算    */
        for(j=0;j<Digit;j++)
        {
            if(Data[j]>10)
            {
                Data[j+1]+=Data[j]/10; /*前一位数组值＝前一位数组值＋此位数组值除以10*/
                Data[j]=Data[j]%10;    /*此位数组值＝此位数组值除10取余数*/
            }
        }
       while(Data[Digit]>=10)//处理最后一位
		{
			Data[Digit+1]+=Data[Digit]/10; /*前一位数组值＝前一位数组值＋此位数组值除以10*/
			Data[Digit]=Data[Digit]%10;    /*此位数组值＝此位数组值除10取余数*/
			Digit++;
		}
		if(Data[Digit]>0)
			Digit++;
        
        printf("%d!=",i);
        for(k=Digit-1;k>=0;k--)       /*输出数组中的内容*/
            printf("%d",Data[k]);
        printf("\n");
	}

    return 0;
}
