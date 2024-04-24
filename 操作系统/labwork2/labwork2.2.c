#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int ticket_sum=20;

void *sell_ticket(void *arg)
{
	int i;
	for(i=0;i<20;i++)
	{
		if(ticket_sum>0)
		{ 			
sleep(1);
			printf("sell the %dth\n",20-ticket_sum+1);
			ticket_sum--;
		}
	}
	return 0;
}

int main()
{
		int flag,i;
		pthread_t tids[4];
		for(i=0;i<4;i++)
		{
			flag=pthread_create(&tids[i],NULL,&sell_ticket,NULL);//创建线程
			if(flag)
			{
				printf("pthread create error ,flag=%d",flag);
				return flag;
			}
		}
		sleep(20);
		void *ans;
		for(i=0;i<4;i++)
		{
			flag=pthread_join(tids[i],&ans);//等待线程结束
			if(flag)
			{
				printf("tid=%lu,join erro flag=%d",tids[i],flag);
				return flag;
			}
			printf("ans=0x%llx\n",(unsigned long long)ans);
			}
			return 0;
}
