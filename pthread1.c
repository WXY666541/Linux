#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>

void* fun(void* arg)
{
	int b = (int)arg;//获取传来的值
	printf("b = %d\n",b);
	int i =0;
	for(;i<3;i++)
	{
		printf("fun running\n");
		sleep(1);
	}
}

int main()
{
	int a = 10;
	pthread_t id;
	int res = pthread_create(&id,NULL,fun,(void*)a);
	assert(res == 0);

	a = 20;
	printf("now a = %d\n",a);

	int i = 0;
	for(;i<5;i++)
	{
		printf("main running\n");
		sleep(1);
	}
	exit(0);
}
